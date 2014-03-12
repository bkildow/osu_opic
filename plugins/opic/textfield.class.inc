<?php

class OsuOpic_Textfield implements OsuOpic_NameNumProviderAbstract {

  protected $name;

  function __construct($name) {
    $this->name = $name;
  }

  function getFieldNames() {
    $fields = field_read_fields(array('type' => 'text'));
    return array_keys($fields);
  }
  
  function getNameDotNumber($entity, $field_name) {
    $items = field_get_items($entity->type, $entity, $field_name);
    $field_value = field_view_value($entity->type, $entity, $field_name, $items[0], array());
    dpm($field_value);
    return '';
  }
}