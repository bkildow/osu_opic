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
  
  function getNameDotNumber($entity_type, $entity, $field_name) {
    $items = field_get_items($entity_type, $entity, $field_name);
    $field_value = field_view_value($entity_type, $entity, $field_name, $items[0], array());
    return render($field_value);
  }
}
