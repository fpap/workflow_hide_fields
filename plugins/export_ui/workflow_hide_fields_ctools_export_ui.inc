<?php

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'workflow_hide_fields_preset', // As defined in hook_schema().
  'access' => 'administer workflow_hide_fields', // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/content',
    'menu item' => 'workflow_hide_fields',
    'menu title' => 'Workflow hide fields',
    'menu description' => 'Administer Workflow hide fields presets.',
  ),
  // Define user interface texts.
  'title singular' => t('preset'),
  'title plural' => t('presets'),
  'title singular proper' => t('Workflow hide fields preset'),
  'title plural proper' => t('Workflow hide fields presets'),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'workflow_hide_fields_ctools_export_ui_form',
    'validate' => 'workflow_hide_fields_ctools_export_ui_form_validate',
    'submit' => 'workflow_hide_fields_ctools_export_ui_form_submit',
  ),
);

/**
 * Workflow hide fields settings form.
 *
 * @param $form
 * @param $form_state
 */
function workflow_hide_fields_ctools_export_ui_form(&$form, &$form_state) {
  // Load the admin form include
  module_load_include('inc', 'workflow_hide_fields', 'workflow_hide_fields.admin');

  // Make optionset reference in form_state
  $form_state['preset'] = &$form_state['item'];

  // Load the configuration form
  $form = drupal_retrieve_form('workflow_hide_fields_form_preset_edit', $form_state);
}

/**
 * Validation handler
 */
function workflow_hide_fields_ctools_export_ui_form_validate(&$form, &$form_state) {
  $presets = workflow_hide_fields_preset_load_all();

  $nodeTypes = array();
  $thisPreset = $form['name']['#value'];
  foreach ($presets as $preset) {
    //drupal_set_message($preset->name, 'warning');
    // Ensure the preset is typecast after being loaded from DB
    if ($thisPreset != $preset->name) {
      _workflow_hide_fields_typecast_preset($preset->options);
      $nodeTypes[] = $preset->options['nodeType'];
    }
  }

  $nodeType = $form['options'][0]['nodeType']['#value'];

  if (in_array($nodeType, $nodeTypes))
    form_set_error('nodeType', 'The node type (' . print_r($form['options'][0]['nodeType']['#options'][$nodeType], true) . ') is already set in another preset, please select a nonconfigured node type.');

  if ($nodeType == '_select_an_item') {
    form_set_error('nodeType', t('Please select an item of field Node Type'));
  }
}

/**
 * Submit callback for Custom Formatters settings form.
 *
 * @param $form
 * @param $form_state
 */
function workflow_hide_fields_ctools_export_ui_form_submit(&$form, &$form_state) {
  // Edit the reference to $form_state['optionset'] which will in turn
  // reference $form_state['item'] which is what CTools is looking for.
  $preset = &$form_state['preset'];
  $preset->title = $form_state['values']['title'];

  // Assign the values to the option set
  $preset->options = _workflow_hide_fields_preset_defaults();

  // Save all the values for the optionset
  foreach ($preset->options as $key => $value) {
    if (array_key_exists($key, $form_state['values'])) {
      $preset->options[$key] = $form_state['values'][$key];
    }
  }
}
