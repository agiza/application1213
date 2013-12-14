<?php

/**
 * Implements hook_entity_iframe_properties_alter().
 *
 * $properties (array) - array of iframe element properties
 * $context (string) - where iframe embed is being displayed, defaults to 'display'
 */
function hook_entity_iframe_properties_alter(&$properties, $context) {
  $properties['class'] = 'iframe';
  // when this is rendered in a view, render differently
  if ($context == 'view') {
    $properties['class'] = 'iframe_view';
    if (isset($properties['frameborder'])) {
      unset($properties['frameborder']);
    }
  }
}