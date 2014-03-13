<?php

/**
 * @file
 * This is used if a opic plugin class is not found.
 */

class OsuOpic_Broken implements OsuOpic_NameNumProviderAbstract {

  public function __construct($plugin_name) {
  }

  public function getFieldNames() {
    return array();
  }

  public function getNameDotNumber($entity, $field_name) {
    return '__PLUGIN_ERROR__';
  }
}
