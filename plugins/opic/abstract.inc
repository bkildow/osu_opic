<?php
/**
 * @file
 * Provides an interface that all opic CTools plugins must implement.
 */

/**
 * Interface OsuOpicNameNumProviderInterface
 */
interface OsuOpicNameNumProviderInterface {
  /**
   * Constructor for the plugin.
   *
   * @param string $plugin_name
   *   the name of the plugin
   */
  public function __construct($plugin_name);

  /**
   * Returns an array of field instance names.
   *
   * These will be field instance names that this plugin knows how to handle.
   * E.g. array('field_osu_opic', 'field_osu_body')
   *
   * @return array
   *   array of field names
   */
  public function getFieldNames();

  /**
   * Given an entity type, entity, and field name gets a name.#.
   *
   * @param string $entity_type
   *   the drupal entity type
   * @param StdClass $entity
   *   a drupal entity object
   * @param string $field_name
   *   a field instance field name. should be in the array above.
   *
   * @return string
   *   a name.# string. E.g. kildow.5
   */
  public function getNameDotNumber($entity_type, $entity, $field_name);
}
