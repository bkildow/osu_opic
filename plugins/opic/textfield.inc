<?php
/**
 * @file
 * An opic CTools plugin for text fields.
 *
 * This provides the logic for how to extract a name.# from a text field.
 */

$plugin = array(
  'title' => t('Textfield Provider'),
  'description' => t('A plugin to provide a name.# from a textfield'),
  'class' => 'OsuOpicTextfield',
  'file' => 'textfield.inc',
);

class OsuOpicTextfield implements OsuOpicNameNumProviderInterface {

  protected $name;

  /**
   * Implements OsuOpicNameNumProviderInterface::_construct().
   */
  public function __construct($name) {
    $this->name = $name;
  }

  /**
   * Implements OsuOpicNameNumProviderInterface::getFieldNames().
   */
  public function getFieldNames() {
    $fields = field_read_fields(array('type' => 'text'));
    return array_keys($fields);
  }

  /**
   * Implements OsuOpicNameNumProviderInterface::getNameDotNumber().
   */
  public function getNameDotNumber($entity_type, $entity, $field_name) {
    $items = field_get_items($entity_type, $entity, $field_name);
    $field_value = field_view_value($entity_type, $entity, $field_name, $items[0], array());
    return render($field_value);
  }
}
