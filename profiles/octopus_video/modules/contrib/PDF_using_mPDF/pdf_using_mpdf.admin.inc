<?php

/**
 * @file
 * Contains the administrative functions of the PDF conversion module.
 *
 * This file is included by the PDF conversion module, and includes the
 * settings form.
 */

/**
 * Admin configure form control on page.
 */
function pdf_using_mpdf_config() {
  if (pdf_using_mpdf_library_exist() == TRUE) {
    $form['pdf'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#title' => t('PDF Tool Option'),
    );
    $form['pdf']['pdf_using_mpdf_pdf_set_author'] = array(
      '#type' => 'textfield',
      '#size' => 35,
      '#title' => t('Author'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_set_author'),
      '#description' => t('Set Author of PDF. If not required, leave blank.'),
    );
    $form['pdf']['pdf_using_mpdf_pdf_set_subject'] = array(
      '#type' => 'textfield',
      '#size' => 35,
      '#title' => t('Subject'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_set_subject'),
      '#description' => t('Set Subject of PDF. If not required, leave blank.'),
    );
    $form['pdf']['pdf_using_mpdf_pdf_save_option'] = array(
      '#type' => 'radios',
      '#title' => t('Open PDF File in'),
      '#options' => array(t('Web Browser'), t('Save Dialog Box')),
      '#default_value' => (variable_get('pdf_using_mpdf_pdf_save_option') == '0') ? 0 : 1,
    );
    $form['pdf']['pdf_using_mpdf_pdf_page_size'] = array(
      '#type' => 'select',
      '#title' => t('Page Size'),
      '#options' => array(
        '2A0' => '2A0', '4A0' => '4A0', 'A0' => 'A0', 'A1' => 'A1', 'A2' => 'A2', 'A3' => 'A3', 'A4' => 'A4',
        'A5' => 'A5', 'A6' => 'A6', 'A7' => 'A7', 'A8' => 'A8', 'A9' => 'A9', 'A10' => 'A10', 'B0' => 'B0', 'B1' => 'B1',
        'B2' => 'B2', 'B3' => 'B3', 'B4' => 'B4', 'B5' => 'B5', 'B6' => 'B6', 'B7' => 'B7', 'B8' => 'B8', 'B9' => 'B9',
        'B10' => 'B10', 'C0' => 'C0', 'C1' => 'C1', 'C2' => 'C2', 'C3' => 'C3', 'C4' => 'C4', 'C5' => 'C5', 'C6' => 'C6',
        'C7' => 'C7', 'C8' => 'C8', 'C9' => 'C9', 'C10' => 'C10', 'RA0' => 'RA0', 'RA1' => 'RA1', 'RA2' => 'RA2',
        'SRA0' => 'SRA0', 'SRA1' => 'SRA1', 'SRA2' => 'SRA2', 'LETTER' => 'Letter', 'LEGAL' => 'Legal',
      ),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_page_size', PDF_USING_MPDF_PDF_PAGE_SIZE),
    );
    $form['pdf']['pdf_using_mpdf_pdf_font_size'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#title' => t('Font Size'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_font_size', PDF_USING_MPDF_PDF_FONT_SIZE),
    );
    $form['pdf']['pdf_using_mpdf_pdf_watermark_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Watermark Text'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_watermark_text'),
      '#description' => t('Display diagonal text on every page of PDF. If not required, leave it blank.'),
    );
    // Setting for PDF header.
    $form['pdf']['header'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('PDF Header Option'),
      '#description' => t('use {PAGENO} for page numbering or {DATE j-m-Y} for current date.'),
    );
    $form['pdf']['header']['pdf_using_mpdf_pdf_header_left'] = array(
      '#type' => 'textfield',
      '#title' => t('Left side content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_header_left'),
    );
    $form['pdf']['header']['pdf_using_mpdf_pdf_header_center'] = array(
      '#type' => 'textfield',
      '#title' => t('Center content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_header_center'),
    );
    $form['pdf']['header']['pdf_using_mpdf_pdf_header_right'] = array(
      '#type' => 'textfield',
      '#title' => t('Right side content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_header_right'),
    );
    // Setting for PDF footer.
    $form['pdf']['footer'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('PDF Footer Option'),
      '#description' => t('use {PAGENO} for page numbering or {DATE j-m-Y} for current date.'),
    );
    $form['pdf']['footer']['pdf_using_mpdf_pdf_footer_left'] = array(
      '#type' => 'textfield',
      '#title' => t('Left side content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_footer_left'),
    );
    $form['pdf']['footer']['pdf_using_mpdf_pdf_footer_center'] = array(
      '#type' => 'textfield',
      '#title' => t('Center content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_footer_center'),
    );
    $form['pdf']['footer']['pdf_using_mpdf_pdf_footer_right'] = array(
      '#type' => 'textfield',
      '#title' => t('Right side content'),
      '#description' => t('if not required, leave blank.'),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_footer_right'),
    );

    // Setting password to PDF, if entered.
    $form['pdf']['permission'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#title' => t('PDF Password Protection'),
    );
    $pwd = variable_get('pdf_using_mpdf_pdf_password');
    if (isset($pwd) && $pwd != NULL) {
      $form['pdf']['permission']['msg'] = array(
        '#type' => 'markup',
        '#markup' => t('<p>Password : ******** is already set.</p>'),
      );
      $form['pdf']['permission']['remove_pwd'] = array(
        '#type' => 'checkbox',
        '#title' => t('Remove Password'),
      );
    }
    else {
      $form['pdf']['permission']['pdf_using_mpdf_pdf_password'] = array(
        '#type' => 'password_confirm',
        '#description' => t('If password is not required, leave blank.'),
      );
    }
    // Setting Style Sheets to PDF.
    $form['pdf']['style'] = array(
      '#type' => 'fieldset',
      '#title' => t('Custom Style Sheets for PDF'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('If not required, leave blank.'),
    );
    $form['pdf']['style']['pdf_using_mpdf_pdf_css_file'] = array(
      '#type' => 'textfield',
      '#description' => t('Enter your name of css file, Example: style.css. Place all stylesheets in the same module directory, i.e. "!module_path/". If the file is in a folder in module directory, enter the name with  path to that folder, for example : "css_folder/style.css".', array('!module_path' => drupal_get_path('module', 'pdf_using_mpdf'))),
      '#default_value' => variable_get('pdf_using_mpdf_pdf_css_file'),
    );
    $filename = variable_get('pdf_using_mpdf_pdf_filename');
    $form['pdf']['pdf_using_mpdf_pdf_filename'] = array(
      '#type' => 'textfield',
      '#title' => t('PDF Filename'),
      '#description' => t('If no name given to PDF file, Default filename : !default_filename will be used.', array('!default_filename' => PDF_USING_MPDF_PDF_DEFAULT_FILENAME)),
      '#default_value' => isset($filename) && $filename != NULL ? $filename : PDF_USING_MPDF_PDF_DEFAULT_FILENAME,
    );
    return system_settings_form($form);
  }
  else {
    // Handling error, if mPDF library does not exist.
    $form['pdf'] = array(
      '#type' => 'markup',
      '#markup' => t('<p>No mPDF Library Found in "sites/all/libraries" or "!default_module_path".<p>Please download PHP mPDF PDF generation tool from <a href="http://www.mpdf1.com/">mPDF1.com</a> </p>', array(
          '!default_module_path' => drupal_get_path('module', 'pdf_using_mpdf'),
        )
      ),
    );
  }
  return $form;
}

/**
 * Configuration form Validation check.
 */
function pdf_using_mpdf_config_validate($form, $form_state) {
  // Checking mPDF library existence.
  if (pdf_using_mpdf_library_exist() == TRUE) {
    if (isset($form_state['values']['remove_pwd'])) {
      if ($form_state['values']['remove_pwd'] == '1') {
        variable_set('pdf_using_mpdf_pdf_password', NULL);
      }
    }
    if (!(is_numeric($form_state['values']['pdf_using_mpdf_pdf_font_size']) && $form_state['values']['pdf_using_mpdf_pdf_font_size'] > 1)) {
      form_set_error('font_size', t('Font size should be numeric and greater than 1.'));
    }
  }
}
