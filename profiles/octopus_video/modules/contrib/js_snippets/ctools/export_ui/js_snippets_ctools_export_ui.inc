<?php
/**
 * @file
 * js_snippets_ctools_export_ui.inc
 */


/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'js_snippets_presets',  // As defined in hook_schema().
  'access' => 'administer content',  // Define a permission users must have to access these pages.

  'create access' => 'js_snippets_access',

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/content/js-snippets',
    'menu item' => 'export',
    'menu title' => 'Import/Export',
    'menu description' => 'Administer JS Snippets presets.',
    'items' => array(
      'clone_into' => array(
        'title' => 'Create JS Snippet',
        'path' => 'list/%ctools_export_ui/clone_into',
        'description' => '',
        'page callback' => 'js_snippets_preset_loader',
        'page arguments' => array(5),
        'access callback' => 'js_snippets_access',
        'access arguments' => array('js_snippets_administer'),
        'type' => MENU_LOCAL_ACTION,
      ),
    ),
  ),

  // Define user interface texts.
  'title singular' => t('Preset'),
  'title plural' => t('Presets'),
  'title singular proper' => t('JS Snippet preset'),
  'title plural proper' => t('JS Snippets presets'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'js_snippets_ctools_export_ui_form',
    // 'submit' and 'validate' are also valid callbacks.
  ),

  // Define what you can do with the plugin
  'allowed operations' => array(
    'enable' => FALSE,
    'disable' => FALSE,
    'clone' => FALSE,
    'clone_into' => array(
      'title' => 'Create JS Snippet',
    ),
  ),


);

/**
 * Define the preset add/edit form.
 */
function js_snippets_ctools_export_ui_form(&$form, &$form_state) {

  $preset = &$form_state['item'];

  // See if our add form has a snippet id
  $args = arg();
  if (count($args) === 6 && $args[4] === 'add') {
    $snippet = js_snippets_load($args[5]);
    $preset->snippets = $snippet->asExportArray();
    $form['info']['name']['#default_value'] = $preset->name = $snippet->name;
    $form['info']['description']['#default_value'] = $preset->description = $snippet->description;

  }

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('The human readable name or description of this preset.'),
    '#default_value' => $preset->description,
    '#required' => true,
  );

}