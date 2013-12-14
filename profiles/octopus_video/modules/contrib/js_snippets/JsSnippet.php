<?php
/**
 * @file
 * JsSnippet.php
 */


class JsSnippet extends Entity {


  /*TYPE*/
  const TYPE_FILE = 'file';
  const TYPE_INLINE = 'inline';
  const TYPE_EXTERNAL = 'external';
  const TYPE_SETTING = 'setting';
  const TYPE_INJECTED = 'injected';

  /*SCOPE*/
  const SCOPE_HEADER = 'header';
  const SCOPE_FOOTER = 'footer';

  /*GROUP*/
  const GROUP_LIBRARY = JS_LIBRARY;
  const GROUP_DEFAULT = JS_DEFAULT;
  const GROUP_THEME = JS_THEME;


  public $id;
  public $name = '';
  public $description = '';

  /**
   * @var bool
   */
  public $library;

  /**
   * This is an indicator if we can only insert inline javascript as is the
   * case with a filter.
   *
   * @var bool
   */
  public $inject_only;

  /**
   * Whether to generate a block for this snippet
   *
   * @var bool
   */
  public $add_block;

  /**
   * @var JsSnippetUnit[]
   */
  public $snippets = array();

  /**
   * @var array
   */
  public $data = array();

  /**
   * Create a stub snippet unit.
   *
   * @return JsSnippetUnit
   */
  public function addSnippet($values = array()) {

    $snippet = new JsSnippetUnit($values);

    if ($this->inject_only) {
      $snippet->type = static::TYPE_INJECTED;
    }

    $this->snippets[] = $snippet;

    return $snippet;
  }

  public function ensureInline() {
    foreach ($this->snippets as $snip) {
      $snip->type = static::TYPE_INJECTED;
    }
  }

  /**
   * Pass an element and all snippet units will be attached to the element
   * invoking the Drupal Javascript API.
   *
   * @param $element
   */
  public function asAttached(&$element) {
    foreach ($this->snippets as $snippet) {
      $snippet->getAsAttached($element);
    }
    $this->addDependencies($element);
  }


  public function addDependencies(&$element) {
    $dependencies = array_filter($this->data['dependencies']);
    if ($dependencies) {
      foreach ($dependencies as $dependency) {
        list($module, $library) = explode(":", $dependency);
        $element['#attached']['library'][] = array($module, $library);
      }
    }
  }

  /**
   * Return an HTML representation (markup) of the Snippet.  The Drupal
   * Javascript API will not be invoked.
   *
   * @return string
   */
  public function asHtml() {
    $element = array();
    $this->asAttached($element);
    return drupal_render($element);
  }


  public function asExportArray() {
    $vars = get_object_vars($this);
    $snippets = array();
    foreach ($vars['snippets'] as $snippet) {
      $snippets[] = get_object_vars($snippet);
    }
    $vars['snippets'] = $snippets;

    // Unset Entity Values
    unset($vars['id']);
    unset($vars['entityType']);
    unset($vars['entityInfo']);
    unset($vars['idKey']);
    unset($vars['nameKey']);
    unset($vars['statusKey']);
    unset($vars['defaultLabel']);

    return $vars;
  }

}


/**
 * Class JsSnippetUnit
 *
 * Basically a wrapper for drupal_add_js
 */
class JsSnippetUnit {

  /**
   * @var string
   */
  public $data;

  /**
   * @var string
   */
  public $type = JsSnippet::TYPE_FILE;

  /**
   * @var string
   */
  public $scope = JsSnippet::SCOPE_HEADER;

  /**
   * @var int
   */
  public $group = JsSnippet::GROUP_DEFAULT;

  /**
   * @var bool
   */
  public $every_page = FALSE;

  /**
   * @var int
   */
  public $weight = 0;

  /**
   * @var bool
   */
  public $cache = TRUE;

  /**
   * @var bool
   */
  public $defer = FALSE;

  /**
   * @var bool
   */
  public $preprocess = TRUE;


  function __construct($values = array()) {
    foreach ($values as $key => $value) {
      $this->{$key} = $value;
    }
  }


  public function getRenderedData() {
    // @TODO: when support JS settings, decide how to handle data payload
    return $this->data;
  }

  public function getFormDefaultData($type) {
    if ($type === $this->type) {
      return $this->data;
    }
    return '';
  }


  public function getAsAttached(&$element) {

    if ($this->type === JsSnippet::TYPE_INJECTED) {
      $element['#markup'] = isset($element['#markup']) ? $element['#markup'] . $this->getRenderedData() : $this->getRenderedData();
    }
    else {
      $element['#attached']['js'][] = array(
        'data'        => $this->getRenderedData(),
        'type'        => $this->type,
        'scope'       => $this->scope,
        'group'       => $this->group,
        'every_page'  => $this->every_page,
        'weight'      => $this->weight,
        'preprocess'  => $this->preprocess,
        'cache'       => $this->cache,
        'defer'       => $this->defer,
      );
    }

  }

  public function addJs() {
    $options = array(
      'type'        => $this->type,
      'scope'       => $this->scope,
      'group'       => $this->group,
      'every_page'  => $this->every_page,
      'weight'      => $this->weight,
      'preprocess'  => $this->preprocess,
      'cache'       => $this->cache,
      'defer'       => $this->defer,
    );

    drupal_add_js($this->getRenderedData(), $options);
  }
}




class JsSnippetController extends EntityAPIController {


  public static $snippetDefaults = array(
    'name' => 'Snippet Name',
    'description' => '',
    'library' => TRUE,
    'inject_only' => FALSE,
    'add_block' => FALSE,
    'snippets' => array(),
    'data' => array(
      'dependencies' => array(),
    ),
  );

  public static $snippetUnitDefaults = array(
    'data'        => '',
    'type'        => JsSnippet::TYPE_FILE,
    'scope'       => JsSnippet::SCOPE_HEADER,
    'group'       => JsSnippet::GROUP_DEFAULT,
    'every_page'  => FALSE,
    'weight'      => 0,
    'preprocess'  => TRUE,
    'cache'       => TRUE,
    'defer'       => FALSE,
  );


  /**
   * Implements EntityAPIControllerInterface.
   */
  public function create(array $values = array()) {
    // Add is_new property if it is not set.
    $values += array('is_new' => TRUE);

    if (isset($values['snippets']) && is_array($values['snippets'])) {
      if (empty($values['snippets'])) {

      }
      else {
        $snippets = $values['snippets'];
        $values['snippets'] = array();
        foreach ($snippets as $snippet) {
          $values['snippets'][] = new JsSnippetUnit($snippet);
        }
      }
    }

    if (isset($this->entityInfo['entity class']) && $class = $this->entityInfo['entity class']) {
      return new $class($values, $this->entityType);
    }
    return (object) $values;
  }

}

class JsSnippetUIController extends EntityDefaultUIController {

  protected $identifier;
  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $items = parent::hook_menu();
    $items[$this->path . '/list']['title'] = 'Snippet Library';
    $items[$this->path . '/untracked'] = array(
      'title' => t('Untracked Snippets'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_overview_form', $this->entityType),
      'description' => 'Manage Untracked Snippets.',
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType),
      'file' => 'includes/entity.ui.inc',
      'type' => MENU_LOCAL_TASK,
    );

    return $items;

  }
  /**
   * Generates the render array for a overview table for arbitrary entities
   * matching the given conditions.
   *
   * @param $conditions
   *   An array of conditions as needed by entity_load().

   * @return Array
   *   A renderable array.
   */
  public function overviewTable($conditions = array(), $identifier = 'entity-overview') {

    $this->identifier = $identifier;
    $untracked = arg(3);

    if ($untracked == 'untracked' && $identifier == 'entity-overview') {
      $conditions['library'] = 0;
    }
    else { //if ($identifier == 'entity-overview') {
      $conditions['library'] = 1;
    }

    $render = parent::overviewTable($conditions);

    $render += array(
      '#prefix' => '<div id="' . $this->identifier . '">',
      '#suffix' => '</div>',
    );

    return $render;
  }

  /**
   * Generates the table headers for the overview table.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {

    $header = array();
    $header[] = 'Name';
    $header[] = 'Description';
    $header[] = 'Embed Only';

    // Add operations with the right colspan.
    $header[] = array('data' => t('Operations'), 'colspan' => 2);
    return $header;
  }

  /**
   * Generates the row for the passed entity and may be overridden in order to
   * customize the rows.
   *
   * @param $additional_cols
   *   Additional columns to be added after the entity label column.
   */
  protected function overviewTableRow($conditions, $id, $snippet, $additional_cols = array()) {

    $row = array();
    /** @var $snippet JsSnippet */
    $row['name'] = $snippet->name;
    $row['description'] = $snippet->description;
    $row['embed_only'] = $snippet->inject_only ? "Embed Only" : '';

    if ($this->identifier == 'entity-overview') {
      $row[] = l(t('edit'), $this->path . '/manage/' . $snippet->id);
      $row[] = l(t('export as preset'), 'admin/content/js-snippets/export/add/' . $snippet->id);
    }
    else {
      $row['select'] = l('Select', 'js_snippet/rendered/' . $snippet->id, array('alias' => TRUE, 'attributes' => array('title' => $snippet->name)));
    }

    return $row;
  }

}

