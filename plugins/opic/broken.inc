<?php

/**
 * @file
 * This is used if a opic plugin class is not found.
 */

class OsuOpicBroken implements OsuOpicNameNumProviderInterface {

  /**
   * Implements OsuOpicNameNumProviderInterface::_construct().
   */
  public function __construct($plugin_name) {
  }

  /**
   * Implements OsuOpicNameNumProviderInterface::getFieldNames().
   */
  public function getFieldNames() {
    return array();
  }

  /**
   * Implements OsuOpicNameNumProviderInterface::getNameDotNumber().
   */
  public function getNameDotNumber($entity_type, $entity, $field_name) {
    return '';
  }
}
