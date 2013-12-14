<?php
/**
 * @file
 * js_snippet.default_snippets.inc
 */

/**
 * Implements hook_default_js_snippets().
 */
function js_snippets_default_js_snippets() {
  $export = array();

  $js_snippets = new stdClass();
  $js_snippets->disabled = FALSE; /* Edit this to true to make a default js_snippets disabled initially */
  $js_snippets->api_version = 1;
  $js_snippets->name = 'js_snippets_dialog_preset';
  $js_snippets->human_name = 'Dialog Preset';
  $js_snippets->description = 'jQuery UI Dialog preset';
  $js_snippets->snippets = array(
    'name' => 'jQuery Dialog',
    'description' => 'jQuery Dialog Snippet',
    'library' => '1',
    'inject_only' => '0',
    'add_block' => '0',
    'snippets' => array(
      0 => array(
        'data' => '<div id="dialog" title="Basic dialog">
  <p>This is the default dialog.</p>
</div>',
        'type' => 'injected',
        'scope' => 'header',
        'group' => '0',
        'every_page' => 0,
        'weight' => '0',
        'cache' => 1,
        'defer' => 0,
        'preprocess' => 0,
      ),
      1 => array(
        'data' => 'jQuery(document).ready(function($) {
  $( "#dialog" ).dialog();
});',
        'type' => 'inline',
        'scope' => 'footer',
        'group' => '0',
        'every_page' => 0,
        'weight' => '0',
        'cache' => 1,
        'defer' => 0,
        'preprocess' => 0,
      ),
    ),
    'data' => array(
      'dependencies' => array(
        'system:ui.dialog' => 'system:ui.dialog',
      ),
    ),
  );

  $export['js_snippets_dialog_preset'] = $js_snippets;

  return $export;

}