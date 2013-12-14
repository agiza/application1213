<?php
/**
 * Declare our snippet plugin.
 * @file js_snippet.inc
 */

/**
 * Implemments hook_PLUGINNAE_plugin
 */
function js_snippets_js_snippet_plugin() {
  $plugins['js_snippet'] = array(
    'title' => t('JS Snippet plugin'),
    'icon title' => t('Insert Javascript Snippet'),
    'settings' => array(
      'dialog' => array(
        'url' => '/wysiwyg/js_snippet/',
        'width' => 1000,
        'height' => 600,
      ),
    ),
    // TinyMCE-specific: Additional HTML elements to allow in the markup.
    'extended_valid_elements' => array(),
  );
  return $plugins;
}
