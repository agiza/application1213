<?php
/**
 * @file
 * js_snippets.admin.inc
 */


function js_snippets_form($form, &$form_state, JsSnippet $snippet, $op = 'add') {

  $form = array(
    '#prefix' => '<div id="js-snippets-wrapper">',
    '#suffix' => '</div>',
  );

  $form['#attached']['js'][] = drupal_get_path('module', 'js_snippets') . '/js_snippets.admin.js';

  if (!isset($form_state['snippet'])) {
    if (empty($snippet->snippets)) {
      $snippet->addSnippet();
    }
    $form_state['snippet'] = $snippet;
  }
  else {
    $snippet = $form_state['snippet'];
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Snippet Identifier'),
    '#description' => t('A short text string to remember the snippet'),
    '#default_value' => $snippet->name,
  );

  if ($op == 'add') {
    $form['library'] = array(
      '#type' => 'hidden',
      '#value' => $snippet->library = TRUE,  // Does not make sense to add a script with add form if you are not going to use it from the library
    );
  }
  else {
    $form['library'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add to Library'),
      '#description' => t('Tick to add to library if you plan to use this snippet again.'),
      '#default_value' => $snippet->library,
    );
  }

  $form['library_info'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('Library Info'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['library_info']['description'] = array(
    '#type' => 'textarea',
    '#required' => FALSE,
    '#title' => t('Description of the Snippet'),
    '#description' => t('A Longer description describing the purpose of the snippet'),
    '#cols' => 60,
    '#rows' => 5,
    '#default_value' => $snippet->description,
  );

  $form['library_info']['add_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide a Block for this Snippet'),
    '#description' => t('If TRUE, a Block will be available for global placement'),
    '#default_value' => $snippet->add_block,
  );

  $form['library_info']['inject_only'] = array(
    '#type' => 'hidden',
    '#value' => $snippet->inject_only,
  );

  $form['library_info']['toggle_inject'] = array(
    '#type' => 'submit',
    '#value' => $snippet->inject_only ? t('Disable Inline Only Setting') : t('Enable Inline Only Setting'),
    '#limit_validation_errors' => array(),
    '#submit' => array('js_snippets_ensure_inject_submit'),
    '#ajax' => array(
      'callback' => 'js_snippets_ensure_inject_js',
      'wrapper' => 'js-snippets-wrapper',
    ),
    '#suffix' => '<span>Tick to ensure that only inline scripts are allowed. This will add snippets to the page as-is without using Drupal JS API</span>',
    '#disabled' => ($op == 'embed'),
  );

  $form['dependencies'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('Dependencies'),
    '#description' => t('Add Library Dependencies'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['dependencies']['dependencies'] = array(
    '#type' => 'checkboxes',
    '#required' => FALSE,
    '#title' => t('Library Dependencies'),
    '#description' => t('Add Library Dependencies'),
    '#default_value' => isset($snippet->data['dependencies']) ? $snippet->data['dependencies'] : array(),
    '#options' => js_snippets_get_available_libraries(),
    '#parents' => array('data', 'dependencies'),
  );


  $form['snippets'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  foreach ($snippet->snippets as $key => $snip) {

    $unit_indicator = (count($snippet->snippets) > 1 ? ' #' . ($key + 1) : '');

    $form['snippets'][$key] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Snippet') . $unit_indicator,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#attributes' => array(
        'class' => array('js-snippets-snippet-wrapper', 'snippet-id-' . $key)
      ),
    );

    if ($snippet->inject_only) {
      $form['snippets'][$key]['type'] = array(
        '#type' => 'hidden',
        '#value' => JsSnippet::TYPE_INJECTED,
        '#attributes' => array(
          'class' => array('js-snippets-snippet-type')
        ),
        '#disabled' => $snippet->inject_only,
      );
    }
    else {
      $form['snippets'][$key]['type'] = array(
        '#type' => 'select',
        '#required' => TRUE,
        '#title' => t('Select type of JS aggregation'),
        '#default_value' => $snip->type,
        '#options' => array(
          JsSnippet::TYPE_EXTERNAL => 'External Javascript',
          JsSnippet::TYPE_FILE => 'Local Javascript',
          JsSnippet::TYPE_INLINE => 'Inline Javascript',
          JsSnippet::TYPE_INJECTED => 'Injected Div and Script',
//      JsSnippet::TYPE_SETTING => 'JavaScript Settings',
        ),
        '#attributes' => array(
          'class' => array('js-snippets-snippet-type')
        ),
      );
    }

    $form['snippets'][$key]['data_external'] = array(
      '#type' => 'textfield',
      '#title' => t('Add exernal Javascript File URL Here'),
      '#description' => t('This URL Should either have full protocol, or the // shorthand for autodetect'),
      '#default_value' => $snip->getFormDefaultData(JsSnippet::TYPE_EXTERNAL),
      '#attributes' => array(
        'class' => array('js-data-input')
      ),
    );
    $form['snippets'][$key]['data_file'] = array(
      '#type' => 'textfield',
      '#title' => t('Add Local Javascript Snippet Here'),
      '#description' => t('The path should be relative to web root'),
      '#default_value' => $snip->getFormDefaultData(JsSnippet::TYPE_FILE),
      '#attributes' => array(
        'class' => array('js-data-input')
      ),
    );

    $form['snippets'][$key]['data_inline'] = array(
      '#type' => 'textarea',
      '#title' => t('Add inline Javascript Snippet Here'),
      '#description' => t('An inline snippet will have the appearce of this type of tag.  Do NOT include script tags'),
      '#cols' => 60,
      '#rows' => 10,
      '#default_value' => $snip->getFormDefaultData(JsSnippet::TYPE_INLINE),
      '#attributes' => array(
        'class' => array('js-data-input')
      ),
    );

    $form['snippets'][$key]['data_injected'] = array(
      '#type' => 'textarea',
      '#title' => t('Add inline Javascript Snippet Here With DOM elements'),
      '#description' => t('This snippet will be added as is to the DOM.  This can be useful for remarketing tags or injecting javascript and div elements into a wysiwyg or a block'),
      '#cols' => 60,
      '#rows' => 10,
      '#default_value' => $snip->getFormDefaultData(JsSnippet::TYPE_INJECTED),
      '#attributes' => array(
        'class' => array('js-data-input')
      ),
    );

//  $form['data_setting'] = array(
//    '#type' => 'textarea',
//    '#required' => TRUE,
//    '#title' => t('Add inline Javascript Snippet Here'),
//    '#description' => t('An inline snippet will have the appearce of this type of tag.  Do NOT include <script> tags'),
//    '#cols' => 60,
//    '#rows' => 10,
//    '#default_value' => $snip->getFormDefaultData($snippet->type),
//  );

    $form['snippets'][$key]['scope'] = array(
      '#type' => 'select',
      '#title' => t('Scope'),
      '#description' => t('Where to attach the Javascript. Header or Footer.  Does not apply to injected JS'),
      '#default_value' => $snip->scope,
      '#options' => array(
        JsSnippet::SCOPE_FOOTER => 'Footer',
        JsSnippet::SCOPE_HEADER => 'Header',
      ),
      '#states' => array(
        'invisible' => array(
          ':input[name="snippets[' . $key .'][type]"]' => array('value' => JsSnippet::TYPE_INJECTED),
        ),
      ),
    );

    // Advanced
    $form['snippets'][$key]['advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced Options'),
      '#description' => t('Additional options about the placement and execution of the JavaScript'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#states' => array(
        'invisible' => array(
          ':input[name="snippets[' . $key .'][type]"]' => array('value' => JsSnippet::TYPE_INJECTED),
        ),
      ),
      '#parents' => array('snippets', $key),
    );

    $form['snippets'][$key]['advanced']['group'] = array(
      '#type' => 'select',
      '#required' => FALSE,
      '#title' => t('Group'),
      '#description' => t('What category of JS should this be aggregated'),
      '#default_value' => $snip->group,
      '#options' => array(
        JsSnippet::GROUP_DEFAULT => 'Default',
        JsSnippet::GROUP_LIBRARY => 'Library',
        JsSnippet::GROUP_THEME => 'Theme',
      ),
    );
    $form['snippets'][$key]['advanced']['every_page'] = array(
      '#type' => 'checkbox',
      '#title' => t('Every Page'),
      '#description' => t('Whether to indicate that this JS should appear on every page.'),
      '#default_value' => $snip->every_page,
    );
    $form['snippets'][$key]['advanced']['weight'] = array(
      '#type' => 'select',
      '#required' => FALSE,
      '#title' => t('Weight'),
      '#description' => t('Weight inside its group'),
      '#default_value' => $snip->weight,
      '#options' => range(-150, 150, 25),
    );
    $form['snippets'][$key]['advanced']['defer'] = array(
      '#type' => 'checkbox',
      '#title' => t('Defer'),
      '#description' => t('Basically, defer tells the browser to wait "until it\'s ready" before executing the javascript in that script block'),
      '#default_value' => $snip->defer,
    );
    $form['snippets'][$key]['advanced']['cache'] = array(
      '#type' => 'checkbox',
      '#title' => t('Cache'),
      '#description' => t('If set to FALSE, the JavaScript file is loaded anew on every page call'),
      '#default_value' => $snip->cache,
    );
    $form['snippets'][$key]['advanced']['preprocess'] = array(
      '#type' => 'checkbox',
      '#title' => t('PreProcess'),
      '#description' => t('If TRUE and JavaScript aggregation is enabled, the script file will be aggregated'),
      '#default_value' => FALSE,
    );

    $form['snippets'][$key]['add_another'] = array(
      '#type' => 'submit',
      '#value' => t('Add Another'),
      '#name' => 'add-another-' . $key,
      '#limit_validation_errors' => array(),
      '#submit' => array('js_snippets_add_another_submit'),
      '#ajax' => array(
        'callback' => 'js_snippets_add_another_js',
        'wrapper' => 'js-snippets-wrapper',
      ),
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );

  if ($op == 'embed') {
    drupal_add_library('system', 'ajax');
    $form['actions']['submit']['#submit'] = array('js_snippets_form_submit');
    $form['actions']['submit']['#ajax'] = array(
      'callback' => 'js_snippets_embed_submit_js',
      'wrapper' => 'js-snippets-wrapper',
    );
  }

  return $form;
}

function js_snippets_form_validate($form, &$form_state) {

  // Must have at least one data full
  $has_data = FALSE;
  foreach ($form_state['values']['snippets'] as $snippet) {

    if ($has_data) {
      break;
    }

    switch ($snippet['type']) {
      case JsSnippet::TYPE_EXTERNAL:
        $has_data = !empty($snippet['data_external']);
        break;
      case JsSnippet::TYPE_FILE:
        $has_data = !empty($snippet['data_file']);
        break;
      case JsSnippet::TYPE_INLINE:
        $has_data = !empty($snippet['data_inline']);
        break;
      case JsSnippet::TYPE_INJECTED:
        $has_data = !empty($snippet['data_injected']);
        break;
    }

  }

  if (!$has_data) {
    form_set_error("", "At least one snippet must have data");
  }


}

function js_snippets_form_submit($form, &$form_state) {
  $snippet = js_snippet_submit_build_snippet($form, $form_state);
  $snippet->save();
  $form_state['redirect'] = 'admin/content/js-snippets';
}


function js_snippets_ensure_inject_submit($form, &$form_state) {
  /** @var $snippet JsSnippet */
  $snippet = &$form_state['snippet'];
  $snippet->inject_only = !$snippet->inject_only;
  $snippet->ensureInline();
  $form_state['rebuild'] = TRUE;
}

function js_snippets_ensure_inject_js($form, &$form_state) {
  return array('messages' => array('#markup' => theme('status_messages')), 'form' => $form);
}


function js_snippets_add_another_submit($form, &$form_state) {
  /** @var $snippet JsSnippet */
  $snippet = &$form_state['snippet'];
  $snippet->addSnippet();
  $form_state['rebuild'] = TRUE;
}

function js_snippets_add_another_js($form, &$form_state) {
  return $form;
}

/**
 * Embed submit JS callback.  Return the snippet to the WYSIWYG editor
 */
function js_snippets_embed_submit_js($form, &$form_state) {
  $commands = array();

  $commands[] = ajax_command_prepend(NULL, theme('status_messages'));
  /** @var $snippet JsSnippet */
  $snippet = $form_state['snippet'];

  $commands[] =   array(
    'command' => 'insertSnippet',
    'id' => $snippet->id,
    'title' => $snippet->name,
    'data' => $snippet->asHtml(),
  );

  return array('#type' => 'ajax', '#commands' => $commands);
}


/**
 * @return JsSnippet|bool
 */
function js_snippet_submit_build_snippet($form, $form_state) {

  form_state_values_clean($form_state);
  $snippet = &$form_state['snippet'];

  foreach ($form_state['values'] as $property => $value) {
    switch ($property) {
      case 'snippets':
        $snippet->snippets = array();
        foreach ($value as $snip) {
          if ($s = _js_snippet_process_snippet_values($snip)) {
            $snippet->snippets[] = $s;
          }
        }
        break;
      case 'data':
        if (isset($value['dependencies'])) {
          $snippet->data['dependencies'] = array_filter($value['dependencies']);
          unset($value['dependencies']);
        }
        $snippet->data += $value;
        break;
      default:
      $snippet->{$property} = $value;
      break;
    }
  }

  return $snippet;
}

function _js_snippet_process_snippet_values($snip) {
  $s = new JsSnippetUnit();
  $values = array(
    'type',
    'scope',
    'group',
    'every_page',
    'weight',
    'defer',
    'cache',
    'preprocess',
  );
  foreach ($values as $value) {
    $s->{$value} = $snip[$value];
  }

  switch ($s->type) {
    case JsSnippet::TYPE_EXTERNAL:
      $s->data = $snip['data_external'];
      break;
    case JsSnippet::TYPE_FILE:
      $s->data = $snip['data_file'];
      break;
    case JsSnippet::TYPE_INLINE:
      $s->data = $snip['data_inline'];
      break;
    case JsSnippet::TYPE_INJECTED:
      $s->data = $snip['data_injected'];
      break;
  }

  if (empty($s->data)) {
    return FALSE;
  }
  return $s;
}
