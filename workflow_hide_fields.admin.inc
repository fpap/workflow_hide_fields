<?php
/**
 * @file
 * Workflow hide fields Administration functionality.
 */
function ajax_fields_callback($form, $form_state) {
  return $form['options'][0]['fields'];
}

function _ajax_get_fields_options($nodetype = '') {
  if (isset($nodetype)) {
    $data = field_info_instances("node", $nodetype);
    $fields = array();
    foreach ($data as $key => $value) {
      $fields[$key] = $value['label'];
    }

    $types = node_type_get_types();

    if (isset($types[$nodetype]) && $types[$nodetype]->has_title) {
      $fields['title'] = $types[$nodetype]->title_label;
    }

    return $fields;
  }
  else {
    return array();
  }
}

/**
 * Defines the form elements used to edit the Workflow hide fields options
 *
 * @param array $options [optional]
 *  Pass in a set of default values for the options
 * @return array
 *  Returns the option set array
 */
function workflow_hide_fields_option_elements($options = array()) {
  $form = array();

  // General Settings
  $form['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
  );

  $nodeTypes['_select_an_item'] = t('Please select an item');
  $nodeTypes = array_merge($nodeTypes, node_type_get_names());
  $form['general_settings']['nodeType'] = array(
    '#type' => 'select',
    '#title' => t('Node type'),
    '#multiple' => FALSE,
    '#description' => t('The Node type'),
    '#options' => $nodeTypes,
    '#default_value' => $options['nodeType'],
    // Bind an ajax callback to the change event (which is the default for the
    // select form type) of the first dropdown. It will replace the second
    // dropdown when rebuilt.
    '#ajax' => array(
      // When 'event' occurs, Drupal will perform an ajax request in the
      // background. Usually the default value is sufficient (eg. change for
      // select elements), but valid values include any jQuery event,
      // most notably 'mousedown', 'blur', and 'submit'.
      'event' => 'change',
      'callback' => 'ajax_fields_callback',
      'wrapper' => 'fields-replace',
    ),
  );
  
  // General Settings
  $form['general_settings']['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields'),
    // The entire enclosing div created here gets replaced when dropdown_first
    // is changed.
    '#prefix' => '<div id="fields-replace">',
    '#suffix' => '</div>',
  );

  $form['general_settings']['fields']['fieldResponsible'] = array(
    '#type' => 'select',
    '#title' => t('Field Responsible'),
    '#description' => t('The Field Responsible'),
//////    '#element_validate' => array('_elexslider_validate_positive_integer'),
    '#default_value' => isset($options['fieldResponsible']) ? $options['fieldResponsible'] : '',
  );

  $form['general_settings']['fields']['fieldRating'] = array(
    '#type' => 'select',
    '#title' => t('Field Rating'),
    '#description' => t('The Field Rating'),
//////    '#element_validate' => array('_elexslider_validate_positive_integer'),
    '#default_value' => isset($options['fieldRating']) ? $options['fieldRating'] : '',
  );

  $form['general_settings']['fields']['fieldState'] = array(
    '#type' => 'select',
    '#title' => t('Field State'),
    '#description' => t('The Field State'),
//////    '#element_validate' => array('_elexslider_validate_positive_integer'),
    '#default_value' => isset($options['fieldState']) ? $options['fieldState'] : '',
  );

  $form['general_settings']['fields']['fieldsWritableOnlyOnCreation'] = array(
    '#type' => 'select',
    '#title' => t('Fields Writable Only On Creation'),
    '#description' => t('The Fields Writable Only On Creation'),
//////    '#element_validate' => array('_elexslider_validate_positive_integer'),
    '#default_value' => isset($options['fieldsWritableOnlyOnCreation']) ? $options['fieldsWritableOnlyOnCreation'] : '',
    '#multiple' => 'true',
  );


  $form['general_settings']['stateResponsibleAssignment'] = array(
    '#type' => 'textfield',
    '#title' => t('State Responsible Assignment'),
    '#description' => t('The State Responsible Assignment'),
//////    '#element_validate' => array('_elexslider_validate_positive_integer'),
    '#default_value' => $options['stateResponsibleAssignment'],
    '#size' => 30,
  );
  $form['general_settings']['statesFinal'] = array(
    '#type' => 'textfield',
    '#title' => t('States Final'),
    '#description' => t('The States Final'),
//////    '#element_validate' => array('_elexslider_validate_positive_integer'),
    '#default_value' => $options['statesFinal'],
    '#size' => 30,
  );
  $form['general_settings']['statesRating'] = array(
    '#type' => 'textfield',
    '#title' => t('States Rating'),
    '#description' => t('The States where the author rates the task'),
//////    '#element_validate' => array('_elexslider_validate_positive_integer'),
    '#default_value' => $options['statesRating'],
    '#size' => 30,
  );
  $form['general_settings']['statesResponsibleAssigned'] = array(
    '#type' => 'textfield',
    '#title' => t('States Responsible Assigned'),
    '#description' => t('The States Responsible Assigned'),
//////    '#element_validate' => array('_elexslider_validate_positive_integer'),
    '#default_value' => $options['statesResponsibleAssigned'],
    '#size' => 30,
  );
  $form['general_settings']['roleSupervisor'] = array(
    '#type' => 'textfield',
    '#title' => t('Role Supervisor'),
    '#description' => t('The Role Supervisor'),
//////    '#element_validate' => array('_elexslider_validate_positive_integer'),
    '#default_value' => $options['roleSupervisor'],
    '#size' => 30,
  );

  return $form;
}

/**
 * Form builder; Form to edit a given option set.
 */
function workflow_hide_fields_form_preset_edit($form, &$form_state) {

  if (empty($form_state['preset'])) {
    $preset = workflow_hide_fields_preset_create();
  }
  else {
    $preset = $form_state['preset'];
  }

  // Title
  $form['title'] = array(
    '#type' => 'textfield',
    '#maxlength' => '255',
    '#title' => t('Title'),
    '#description' => t('A human-readable title for this option set.'),
    '#required' => TRUE,
    '#default_value' => $preset->title,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => '255',
    '#machine_name' => array(
      'source' => array('title'),
      'exists' => 'workflow_hide_fields_preset_exists',
    ),
    '#required' => TRUE,
    '#default_value' => $preset->name,
  );


  // Options Vertical Tab Group table
  $form['preset'] = array(
    '#type' => 'vertical_tabs',
  );

  $default_options = workflow_hide_fields_option_elements($preset->options);
  // Add the options to the vertical tabs section
  foreach ($default_options as $key => $value) {
    $form['options'][] = $value;
  }

  if (!empty($form_state['values']['nodeType'])) {
    $selected = $form_state['values']['nodeType'];

    // When the form is rebuilt during ajax processing, the $selected variable
    // will now have the new value and so the options will change.    
    $form['options'][0]['fields']['fieldResponsible']['#options'] = _ajax_get_fields_options($selected);
    $form['options'][0]['fields']['fieldRating']['#options'] = _ajax_get_fields_options($selected);
    $form['options'][0]['fields']['fieldState']['#options'] = _ajax_get_fields_options($selected);
    $form['options'][0]['fields']['fieldsWritableOnlyOnCreation']['#options'] = _ajax_get_fields_options($selected);
  }
  else {
    $form['options'][0]['fields']['fieldResponsible']['#options'] = _ajax_get_fields_options($form['options'][0]['nodeType']['#default_value']);
    $form['options'][0]['fields']['fieldRating']['#options'] = _ajax_get_fields_options($form['options'][0]['nodeType']['#default_value']);
    $form['options'][0]['fields']['fieldState']['#options'] = _ajax_get_fields_options($form['options'][0]['nodeType']['#default_value']);
    $form['options'][0]['fields']['fieldsWritableOnlyOnCreation']['#options'] = _ajax_get_fields_options($form['options'][0]['nodeType']['#default_value']);
  }

  return $form;
}

/**
 *
 */
function workflow_hide_fields_settings_form($form, $form_state) {
  $settings = variable_get('workflow_hide_fields_settings', array('checkbox' => TRUE)); //, 'label_prefix_value' => t('Workflow')));

  $form['settings'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $form['settings']['checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check it?'),
    '#description' => t('Just a checkbox.'),
    '#default_value' => $settings['checkbox'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}
