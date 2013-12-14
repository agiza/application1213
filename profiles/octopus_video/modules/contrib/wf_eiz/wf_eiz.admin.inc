<?php 
function wf_eiz_settings_page($op = NULL) {
  $output = drupal_get_form('wf_eiz_general_settings_form');
  return $output;
} // wf_eiz_settings_page


function wf_eiz_general_settings_form() {
  // Define layout fieldset.
  /* --------------------------------- */
  $form['layout_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Epic Image Zoom Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  // Define  fieldset.
  /* ------------------------------- */
  $use_lite = variable_get('wf_eiz_lite', FALSE);
  $form['layout_fieldset']['wf_eiz_lite_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('wf_eiz lite'),
    '#collapsible' => TRUE,
    '#collapsed' => !$use_lite,
  );

  // Add checkbox for wf_eiz .
  $form['layout_fieldset']['wf_eiz_lite_options']['wf_eiz_lite']  = array(
    '#type' => 'checkbox',
    '#title' => t('Use Epic Image Zoom'),
    '#description' => t('Checking this box will enable Epic Image Zoom.'),
    '#default_value' => $use_lite,
  );
  
    return system_settings_form($form);
} // wf_eiz_general_settings_form