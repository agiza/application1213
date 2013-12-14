<?php
/**
 * @file
 * Draggable Images Admin
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Admin Form: UI
 *
 */
function draggable_images_admin($form, &$form_state, $op = NULL) {

  global $_draggable_images_set;

  $settings = $_draggable_images_set;

  $form = array();

  $form['draggable_images'] = array(
    '#type'          => 'fieldset',
    '#title'         => 'Draggable Images Configuration',
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
    '#tree'          => TRUE,
  );

  $form['draggable_images']['enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enabled'),
    '#default_value' => $settings['enabled'],
    '#description'   => t('Enable/Disable Draggable Images (Useful for testing)'),
    '#prefix'        => '<div class="draggable-images-columns clearfix"><div class="draggable-images-column draggable-images-column-1">',
  );

  $form['draggable_images']['position'] = array(
    '#type'          => 'radios',
    '#title'         => t('Position'),
    '#default_value' => !empty($settings['position']) ? $settings['position']: 0,
    '#options'       => array( 'fixed' => 'Fixed', 'absolute' => 'Absolute'),
    '#description'   => t('Fixed - stays on screern, Absolute - scrolls.'),
  );

  $form['draggable_images']['vertical'] = array(
    '#type'          => 'select',
    '#title'         => t('Vertical'),
    '#default_value' => !empty($settings['vertical']) ? $settings['vertical']: 0,
    '#options'       => array('top' => 'Top', 'bottom' => 'Bottom'),
    '#description'   => t('Vertical position'),
  );

  $form['draggable_images']['horizontal'] = array(
    '#type'          => 'select',
    '#title'         => t('Horizontal'),
    '#default_value' => !empty($settings['horizontal']) ? $settings['horizontal']: '',
    '#options'       => array('left' => 'Left', 'right' => 'Right'),
    '#description'   => t('Horizontal position'),
  );

  $form['draggable_images']['width'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Width'),
    '#default_value' => !empty($settings['width']) ? $settings['width']: 960,
    '#description'   => t('Draggable width area.'),
  );

  $form['draggable_images']['height'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Height'),
    '#default_value' => !empty($settings['height']) ? $settings['height']: 400,
    '#description'   => t('Draggable height area.'),
  );
      
  $options = image_style_options(TRUE);

  $form['draggable_images']['image_style'] = array(
    '#type'          => 'select',
    '#title'         => t('Image Style'),
    '#default_value' => $settings['image_style'],
    '#options'       => $options,
    '#suffix'        => '</div><div class="draggable-images-column draggable-images-column-2">',
  );

  $form['draggable_images']['images'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Images'),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix'        => '<div id="images-fieldset-wrapper">',
    '#suffix'        => '</div>',
    '#description'     => t('For best result, upload images with minimum dimensions: 300px width and 300px height.'),
  );

  $images = isset($settings['images']) ? $settings['images']: array();
  // for ajax call
  $image_count = count($images);
  if (empty($form_state['image_num'])) $form_state['image_num'] = $image_count;
  
  if ($images != 0) {

    for ($i = 0; $i < $form_state['image_num']; $i++) {
      
      if (isset($images['dimage' . $i])) {
        $image_fid = $images['dimage' . $i];
        $image = file_load($image_fid);
        $image_source = '<img src="' . image_style_url('thumbnail', $image->uri) . '" />';
      }
      else {
        $image_source = '';      
      }
      
      $form['draggable_images']['images']['dimage' . $i] = array(
        //'#title'           => t('Image'),
        '#type'            => 'managed_file',
        '#description'     => $image_source,
        '#default_value'   => isset($images['dimage' . $i]) ? $images['dimage' . $i]: '',
        '#upload_location' => 'public://draggable_images/',
        '#progress_indicator' => 'bar',
      );
      $image_source = '';
    }
  }

  $form['draggable_images']['add_another'] = array(
    '#type'      => 'submit',
    '#value'     => t('Add another image'),
    '#submit'    => array('draggable_images_add_another'),
    '#ajax'      => array(
      'callback' => 'draggable_images_add_another_callback',
      'wrapper'  => 'images-fieldset-wrapper',
    ),
  );

  $form['draggable_images']['exclude'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Excluded Pages'),
    '#default_value' => $settings['exclude'],
    '#description'   => t('List the page paths to be excluded from Responsive Background Images on each line.'),
    '#suffix'        => '</div></div>',
  );

  $form['save']      = array( '#type' => 'submit', '#value' => 'Save', );
  $form['reset']     = array( '#type' => 'submit', '#value' => 'Reset', );

  //dsm($form);
  return $form;
}


/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function draggable_images_add_another_callback($form, $form_state) {
  return $form['draggable_images']['images'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function draggable_images_add_another($form, &$form_state) {
  $form_state['image_num']++;
  $form_state['rebuild'] = TRUE;
}


/**
 * Admin Form: Submit
 *
 */
function draggable_images_admin_submit($form, $form_state) {
  global $user;
  global $_draggable_images_defaults;

  $form_values = $form_state['values'];
  //dsm($form_values);
  // make the images permanent
  $images = $form_values['draggable_images']['images'];
  if (!empty($images)) {
    foreach ($images as $key => $image_fid) {
      if ($image_fid != 0) {
        // Load the file via file.fid.
        $image = file_load($image_fid);
        // Change status to permanent.
        $image->status = FILE_STATUS_PERMANENT;
        // Save.
        file_save($image);
        // Record that the module (in this example, user module) is using the file.
        file_usage_add($image, 'user', 'user', $user->uid);
      }
      elseif ($image_fid == 0) {
        unset($form_values['draggable_images']['images'][$key]);
      }
    }
  }
  if ($form_values['op'] == 'Save') {
    $settings = $form_values['draggable_images']; // save user settings
  }
  elseif ($form_values['op'] == 'Reset') {
    $settings = $_draggable_images_defaults; // save default settings
  }
  // recreate all image styles
  image_flush_caches();

  variable_set('draggable_images', $settings);
}
