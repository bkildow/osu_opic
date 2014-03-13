<?php

class OsuOpic_FieldSettings extends OsuOpic_PluginWrangler {

  /**
   * Generates a form options array for each opic plugin.
   *
   * @return array
   *   form options array
   */
  function getPluginOptions() {
    // Get all available opic plugins.
    $plugins = $this->getPlugins();

    $options = array('none' => 'None');

    foreach ($plugins as $id => $plugin) {
      $options[$id] = $plugin['title'];
    }

    return $options;
  }

  /**
   * Generates form settings for each opic plugin.
   *
   * @param array $form_state
   *   the form_state array for field settings
   *
   * @return array
   *   drupal form array
   */
  function generateFieldSettings(&$form_state) {
    $plugins = $this->getPlugins();
    $settings = array();

    foreach ($plugins as $id => $plugin) {
      $settings[$id] = array(
        '#type' => 'select',
        '#title' => 'Field that provides a name.#',
        '#default_value' => $this->getPluginFieldDefaultValue($form_state, $id),
        '#options' => $this->getFieldOptions($id),
        '#states' => array(
          // Only show if this plugin was selected
          'visible' => array(
            ':input[name="instance[settings][opic][osu_opic_plugin]"]' => array(
              'value' => $id,
            ),
          ),
        ),
      );
    }

    return $settings;
  }

  /**
   * Gets a default value for the opic plugin field settings
   *
   * @param array $form_state
   *   form_state array
   * @param string $plugin_id
   *   opic plugin id
   *
   * @return string
   *   default value
   */
  function getPluginFieldDefaultValue(&$form_state, $plugin_id) {
    $settings =& $form_state['build_info']['args']['0']['settings'];
    if(isset($settings['opic'][$plugin_id])) {
      return $settings['opic'][$plugin_id];
    }
    else {
      return '';
    }
  }

  /**
   * Gets a default value for the opic plugin selected
   *
   * @param array $form_state
   *   form_state array
   *
   * @return string
   *   default value
   */
  function getPluginDefaultValue(&$form_state) {
    $settings =& $form_state['build_info']['args']['0']['settings'];
    if(isset($settings['opic']['osu_opic_plugin'])) {
      return $settings['opic']['osu_opic_plugin'];
    }
    else {
      return '';
    }
  }

  /**
   * Gets form options per opic plugin.
   *
   * @param string $plugin_id
   *   the id of the opic plugin
   *
   * @return array
   *   Drupal form options array
   */
  private function getFieldOptions($plugin_id) {
    $plugin = $this->getPluginInstance($plugin_id);
    return drupal_map_assoc($plugin->getFieldNames());
  }
}
