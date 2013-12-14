<?php

/**
 * @file
 * Admin page callbacks for the block module.
 *
 * @name Node-level Blocks Admin
 * @author skilip
 */

/**
 * Extra form elements for the node type form.
 */
function node_level_blocks_node_type_form(&$form, &$form_state) {
  $settings = node_level_blocks_node_type_settings($form['#node_type']->type);

  $module_options = array();
  foreach (module_implements('block_info') as $module) {
    $module_options[$module] = $module;
  }

  $form['node_level_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node-level Blocks'),
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
    '#collapsed' => !($settings->enabled),
    '#access' => user_access('administer node_level_blocks'),
  );
  $form['node_level_blocks']['node_level_blocks_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Node-level Blocks functionality'),
    '#description' => '',
    '#default_value' => $settings->enabled,
  );
  $form['node_level_blocks']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        'input[name=node_level_blocks_enabled]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['node_level_blocks']['settings']['node_level_blocks_display_node_edit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display blocks on node-edit pages'),
    '#default_value' => $settings->display_node_edit,
  );
  $form['node_level_blocks']['settings']['node_level_blocks_multi'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow duplication of blocks on node pages'),
    '#default_value' => $settings->multi,
  );
  $form['node_level_blocks']['settings']['node_level_blocks_regions'] = array(
    '#type' => 'select',
    '#title' => t('Enabled regions'),
    '#description' => t('Choose which regions are available for the current content type'),
    '#options' => system_region_list(variable_get('theme_default', 'garland')),
    '#multiple' => TRUE,
    '#default_value' => $settings->regions,
  );
  $form['node_level_blocks']['settings']['node_level_blocks_modules'] = array(
    '#type' => 'select',
    '#title' => t('Modules'),
    '#description' => t('Here you can choose which modules to use for providing block contents.'),
    '#options' => $module_options,
    '#multiple' => TRUE,
    '#default_value' => ($settings->modules ? $settings->modules : $module_options),
  );
  $form['#submit'][] = 'node_level_blocks_node_type_form_submit';
  $form['#attached']['js'][] = drupal_get_path('module', 'node_level_blocks') . '/js/node_level_blocks.js';
}

/**
 * Custom submit callback for the node type form.
 */
function node_level_blocks_node_type_form_submit($form, &$form_state) {
  $settings = (object) array(
    'enabled' => $form_state['values']['node_level_blocks_enabled'],
    'display_node_edit' => $form_state['values']['node_level_blocks_display_node_edit'],
    'multi' => $form_state['values']['node_level_blocks_multi'],
    'regions' => $form_state['values']['node_level_blocks_regions'],
    'modules' => $form_state['values']['node_level_blocks_modules'],
  );
  variable_set('node_level_blocks_enabled_' . $form_state['values']['type'], $settings);
}

/**
 * Extra form elements for the node type form.
 */
function node_level_blocks_fieldset_node_edit_form(&$form, &$form_state) {
  // set default theme for retrieving regions and blocks
  $theme_key = variable_get('theme_default', 'garland');

  // Set default values.
  $settings = isset($form['#node']->node_level_blocks['settings']) ? $form['#node']->node_level_blocks['settings'] : node_level_blocks_node_type_settings($form['#node']->type);

  $submitted = $form_state['submitted'] && isset($form_state['values']['node_level_blocks']);
  $node_blocks = !$submitted && isset($form['#node']->node_level_blocks['blocks']) ? $form['#node']->node_level_blocks['blocks'] : array();

  // If a node is submitted with the 'preview' button,
  // the data can be collected using the form state.
  // Othewise the stored nodes, or default values can be used.
  if ($submitted) {
    if (!isset($form_state['values']['node_level_blocks']['settings'])) {
      $form_state['values']['node_level_blocks']['settings'] = $settings;
    }

    foreach ($form_state['values']['node_level_blocks']['blocks'] as $node_block) {
      $node_blocks[$node_block['bid']] = (object) $node_block;
    }
  }

  // Fetch and sort blocks
  $blocks = _block_rehash($theme_key);

  // The disabled blocks need to be sorted by title instead of weight
  // We will use this array to store all blocks which are not assigned to a snippet region.
  // After collecting all unassigned blocks, we will sort this array by info key
  // and merge it back to the blocks array.
  $blocks_disabled = array();

  // Loop through the blocks array to do some last cleanup stuff.
  foreach ($blocks as $i => $block) {
    // If the blocks for the current block's module has been disabled, unset it.
    if (is_array($settings->modules) && !in_array($block['module'], $settings->modules)) {
      unset($blocks[$i]);
    }
    else {
      if (isset($node_blocks[$block['bid']])) {
        // Overwrite the block weights here to make sure the list is ordered correctly.
        $blocks[$i]['weight'] = $node_blocks[$block['bid']]->weight;
      }
      if (!in_array($block['region'], $settings->regions) && (!isset($node_blocks[$block['bid']]) || !in_array($node_blocks[$block['bid']]->region, $settings->regions))) {
        // The block isn't assigned to a snippet region.
        // Set the weight parameter to a fixed value in order to force sorting by info.
        $block['weight'] = -10;
        // Store the block in a temp array.
        $blocks_disabled[] = $block;
        // Unset the original block.
        unset($blocks[$i]);
      }
    }
  }
  // Sort the enabled blocks first by weight, then by info.
  usort($blocks, '_node_level_blocks_compare');
  // Sort the disabled blocks by info.
  usort($blocks_disabled, '_node_level_blocks_compare');
  $blocks = array_merge($blocks, $blocks_disabled);

  // Overwrite the block weights here to make sure the list is ordered correctly.
  foreach ($blocks as $i => $block) {
    if (isset($node_blocks[$block['bid']])) {
      $blocks[$i]['weight'] = $node_blocks[$block['bid']]->weight;
    }
  }
  usort($blocks, '_node_level_blocks_compare');

  $block_regions = system_region_list($theme_key) + array(BLOCK_REGION_NONE => '<' . t('none') . '>');

  // Remove unused regions for this node type.
  foreach (array_keys($block_regions) as $key) {
    if (!in_array($key, $settings->regions) && $key !== BLOCK_REGION_NONE) {
      unset($block_regions[$key]);
    }
  }

  // Weights range from -delta to +delta, so delta should be at least half
  // of the amount of blocks present. This makes sure all blocks in the same
  // region get an unique weight.
  $weight_delta = round(count($blocks) / 2);

  $form['node_level_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node-level Blocks'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#group' => 'additional_settings',
    '#collapsed' => !($settings->enabled),
    '#access' => ($settings->enabled && user_access('edit node_level_blocks')),
  );

  $form['node_level_blocks']['blocks'] = array(
    '#theme' => 'node_level_blocks_fieldset',
    '#settings' => $settings,
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'block') . '/block.css',
      ),
    ),
  );

  foreach ($blocks as $i => $block) {
    $key = $block['module'] . '_' . $block['delta'];

    // If the node_block's region isn't present in the settings regions,
    // put it in the disabled area, otherwise it won't be visible anymore.
    if (isset($node_blocks[$block['bid']]) && !in_array($node_blocks[$block['bid']]->region, $settings->regions)) {
      $node_blocks[$block['bid']]->region = BLOCK_REGION_NONE;
    }

    $form['node_level_blocks']['blocks'][$key]['module'] = array(
      '#type' => 'value',
      '#value' => $block['module'],
    );
    $form['node_level_blocks']['blocks'][$key]['bid'] = array(
      '#type' => 'value',
      '#value' => $block['bid'],
    );
    $form['node_level_blocks']['blocks'][$key]['delta'] = array(
      '#type' => 'value',
      '#value' => $block['delta'],
    );
    $form['node_level_blocks']['blocks'][$key]['info'] = array(
      '#markup' => check_plain($block['info']),
    );
    $form['node_level_blocks']['blocks'][$key]['theme'] = array(
      '#type' => 'hidden',
      '#value' => $theme_key,
    );
    $form['node_level_blocks']['blocks'][$key]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $block['weight'],
      '#delta' => $weight_delta,
    );
    $form['node_level_blocks']['blocks'][$key]['region'] = array(
      '#type' => 'select',
      '#default_value' => (isset($node_blocks[$block['bid']]) ? $node_blocks[$block['bid']]->region : BLOCK_REGION_NONE),
      '#options' => $block_regions,
    );
  }
}

/**
 * Helper function for sorting blocks on admin/build/block.
 *
 * Active blocks are sorted by region, then by weight.
 * Disabled blocks are sorted by name.
 */
function _node_level_blocks_compare($a, $b) {
  // Sort by weight.
  $weight = $a['weight'] - $b['weight'];
  if ($weight) {
    return $weight;
  }
  // Sort by title.
  return strcmp($a['info'], $b['info']);
}

/**
 * Process variables for block-admin-display.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see block-admin-display.tpl.php
 * @see theme_block_admin_display()
 */
function template_preprocess_node_level_blocks_fieldset(&$variables) {
  // set default theme for retrieving regions and blocks
  $theme_key = variable_get('theme_default', 'garland');

  $settings = $variables['form']['#settings'];

  $block_regions = system_region_list($theme_key);

  $variables['block_regions'] = $block_regions + array(BLOCK_REGION_NONE => t('Disabled'));

  foreach (array_keys($block_regions) as $key) {
    // Initialize an empty array for the region.
    if (in_array($key, $settings->regions)) {
      $variables['block_listing'][$key] = array();
    }
    else {
      unset($variables['block_regions'][$key]);
    }
  }

  // Initialize disabled blocks array.
  $variables['block_listing'][BLOCK_REGION_NONE] = array();

  foreach (element_children($variables['form']) as $i) {
    $block = &$variables['form'][$i];

    // Only take form elements that are blocks.
    if (isset($block['info'])) {
      // Fetch region for current block.
      $region = $block['region']['#default_value'];

      // Set special classes needed for table drag and drop.
      $variables['form'][$i]['region']['#attributes']['class'] = array('block-region-select block-region-' . $region);
      $variables['form'][$i]['weight']['#attributes']['class'] = array('block-weight block-weight-' . $region);

      $variables['block_listing'][$region][$i] = new stdClass();
      $variables['block_listing'][$region][$i]->row_class = !empty($block['#attributes']['class']) ? implode(' ', $block['#attributes']['class']) : '';
      $variables['block_listing'][$region][$i]->block_modified = !empty($block['#attributes']['class']) && in_array('block-modified', $block['#attributes']['class']);
      $variables['block_listing'][$region][$i]->block_title = drupal_render($block['info']);
      $variables['block_listing'][$region][$i]->region_select = drupal_render($block['region']) . drupal_render($block['theme']);
      $variables['block_listing'][$region][$i]->weight_select = drupal_render($block['weight']);
      $variables['block_listing'][$region][$i]->printed = FALSE;
    }
  }
}
