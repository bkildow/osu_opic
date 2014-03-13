<?php

class OsuOpic_PluginWrangler {
  function __construct() {
    ctools_include('plugins');
  }

  /**
   * Gets a list of opic CTools plugins
   *
   * @return array
   *   array of opic CTools plugins
   */
  function getPlugins() {
    return ctools_get_plugins('osu_opic', 'opic');
  }

  /**
   * @param $plugin_id
   * @return OsuOpic_NameNumProviderAbstract
   */
  function getPluginInstance($plugin_id) {
    return osu_opic_get_plugin_object($plugin_id);
  }

  /**
   * Gets the the plugin id from a field_info array
   *
   * @param array $field_info
   *   drupal field_info as returned by field_info_instance().
   *
   * @return string
   *   the plugin id
   */
  function getPluginIdFromFieldInfo($field_info) {
    if (!isset($field_info['settings']['opic']['osu_opic_plugin'])) {
      return '';
    }

    return $field_info['settings']['opic']['osu_opic_plugin'];
  }

  /**
   * Gets the name of the field providing a name.#
   *
   * @param array $field_info
   *   drupal field_info as returned by field_info_instance()
   *
   * @return string
   *   the name of the field providing a name.#
   */
  function getNameDotNumberFieldNameFromFieldInfo($field_info) {
    $plugin_id = $this->getPluginIdFromFieldInfo($field_info);
    if (empty($plugin_id)) {
      return '';
    }

    return $field_info['opic'][$plugin_id];
  }

  /**
   * Loads the CTools opic plugin and calls the name.# handler for the field
   *
   * @param StdClass $entity
   *   drupal entity object that contains the image and name.# field
   * @param array $field_info
   *   the field info array for the image field which has opic settings
   *
   * @return string
   *   a name.# string
   */
  function getNameDotNumberFromPlugin($entity, $field_info) {
    $plugin_id = $this->getPluginIdFromFieldInfo($field_info);
    $field_name = $this->getNameDotNumberFieldNameFromFieldInfo($field_info);
    $plugin = $this->getPluginInstance($plugin_id);
    return $plugin->getNameDotNumber($entity, $field_name);
  }
}
