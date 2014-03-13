<?php

/**
 * @file
 * This is used if a opic plugin class is not found.
 */

class OsuOpicBroken implements OsuOpicNameNumProviderAbstract {

  public function __construct($plugin_name) {
  }

  public function getFieldNames() {
    return array();
  }

  public function getNameDotNumber($entity_type, $entity, $field_name) {
    return '__PLUGIN_ERROR__';
  }
}
