<?php

interface OsuOpic_NameNumProviderAbstract {
  /**
   * Constructor for the plugin.
   *
   * @param $plugin_name
   *   the name of the plugin
   */
  public function __construct($plugin_name);

  /**
   * Returns an array of field instance names that this plugin knows how to
   * handle.
   *
   * E.g. array('field_osu_opic', 'field_osu_body')
   *
   * @return array
   *   array of field names
   */
  public function getFieldNames();

  /**
   * @param StdClass $entity
   *   a drupal entity object
   * @param string $field_name
   *   a field instance field name. should be in the array above.
   *
   * @return string
   *   a name.# string. E.g. kildow.5
   */
  public function getNameDotNumber($entity, $field_name);
}
