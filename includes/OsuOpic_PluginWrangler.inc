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
   * Gets a name.# from the selected opic plugin
   *
   * @param string $entity_type
   *   entity type
   * @param StdClass $entity
   *   entity object
   * @param string $image_field
   *   field name of the image field that the plugin is configured on
   *
   * @return string
   *   a name.#
   */
  function getNameDotNumber($entity_type, $entity, $image_field) {
    $info = field_info_instance($entity_type, $image_field, $entity->type);
    return $this->getNameDotNumberFromPlugin($entity_type, $entity, $info);
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
  private function getPluginIdFromFieldInfo($field_info) {
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
   * @param string $plugin_id
   *   plugin id
   *
   * @return string
   *   the name of the field providing a name.#
   */
  private function getNameDotNumberFieldNameFromFieldInfo($field_info, $plugin_id) {
    if (empty($plugin_id)) {
      return '';
    }

    return $field_info['settings']['opic'][$plugin_id];
  }

  /**
   * Loads the CTools opic plugin and calls the name.# handler for the field
   *
   * @param string $entity_type
   *   drupal entity type
   * @param StdClass $entity
   *   drupal entity object that contains the image and name.# field
   * @param array $field_info
   *   the field info array for the image field which has opic settings
   *
   * @return string
   *   a name.# string
   */
  private function getNameDotNumberFromPlugin($entity_type, $entity, $field_info) {
    $plugin_id = $this->getPluginIdFromFieldInfo($field_info);
    $field_name = $this->getNameDotNumberFieldNameFromFieldInfo($field_info, $plugin_id);
    $plugin = $this->getPluginInstance($plugin_id);
    $name_n = $plugin->getNameDotNumber($entity_type, $entity, $field_name);
    return $name_n;
  }
}
