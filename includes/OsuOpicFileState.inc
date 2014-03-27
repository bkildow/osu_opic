<?php
/**
 * @file
 * Determines file states for an image field.
 */

/**
 * Class OsuOpicFileState
 */
class OsuOpicFileState {
  const IMAGE_EMPTY_UNCHANGED = 1;
  const IMAGE_EXISTS_UNCHANGED = 2;
  const IMAGE_NEW = 3;
  const IMAGE_UPDATED = 4;
  const IMAGE_DELETED = 5;
  const IMAGE_NULL = 6;

  protected $entity;
  protected $entityType;
  protected $imageField;
  protected $currentState;
  protected $origFid;
  protected $newFid;

  /**
   * Constructor.
   *
   * @param string $entity_type
   *   the Drupal entity type
   * @param StdClass $entity
   *   a Drupal entity
   * @param string $image_field
   *   the name of the image field
   */
  public function __construct($entity_type, $entity, $image_field) {
    $this->entity = $entity;
    $this->entityType = $entity_type;
    $this->imageField = $image_field;
    $this->currentState = self::IMAGE_NULL;
    $this->setOrigFid();
    $this->setNewFid();
  }

  /**
   * Gets the current state for uploaded files.
   *
   * @return int
   *   returns one of the constants defined in this class
   */
  public function getState() {
    $this->determineCurrentState();
    return $this->currentState;
  }

  /**
   * Figures what the current state is and sets $this->current_state.
   */
  protected function determineCurrentState() {
    if ($this->isEmptyUnchanged()) {
      $state = self::IMAGE_EMPTY_UNCHANGED;
    }
    elseif ($this->isExistsUnchanged()) {
      $state = self::IMAGE_EXISTS_UNCHANGED;
    }
    elseif ($this->isNew()) {
      $state = self::IMAGE_NEW;
    }
    elseif ($this->isUpdated()) {
      $state = self::IMAGE_UPDATED;
    }
    elseif ($this->isDeleted()) {
      $state = self::IMAGE_DELETED;
    }
    else {
      $state = self::IMAGE_NULL;
    }

    $this->currentState = $state;
  }

  /**
   * Determines if the state is IMAGE_EMPTY_UNCHANGED.
   *
   * @return bool
   *   true if it is, else returns false
   */
  protected function isEmptyUnchanged() {

    // If this is a new entity, and no image was uploaded.
    if ($this->entity->is_new && empty($this->newFid)) {
      return TRUE;
    }

    // If the original and new fields are empty.
    if (empty($this->newFid) && empty($this->origFid)) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Determines if the state is IMAGE_EXISTS_UNCHANGED.
   *
   * @return bool
   *   true if it is, else returns false
   */
  protected function isExistsUnchanged() {

    // If fids match.
    if ($this->newFid == $this->origFid) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Determines if the state is IMAGE_NEW.
   *
   * @return bool
   *   true if it is, else returns false
   */
  protected function isNew() {

    // If this is a new entity, and an image was uploaded.
    if ($this->entity->is_new && !empty($this->newFid)) {
      return TRUE;
    }

    // If there wasn't previously an image, and we are uploading a new one.
    if ((empty($this->origFid)) && !empty($this->newFid)) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Determines if the state is IMAGE_UPDATED.
   *
   * @return bool
   *   true if it is, else returns false
   */
  protected function isUpdated() {

    // If fids exist on both orig and new, and they don't match.
    if ((!empty($this->origFid) && !empty($this->newFid)) && $this->origFid !== $this->newFid) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Determines if the state is IMAGE_DELETED.
   *
   * @return bool
   *   true if it is, else returns false
   */
  protected function isDeleted() {

    // If fid existed on original but not on new.
    if (!empty($this->origFid) && empty($this->newFid)) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Gets a file id from the image field passed into the constructor.
   *
   * @param StdClass $entity
   *   a Drupal entity
   *
   * @return null|int
   *   a FID or null if one is not set
   */
  protected function getFidFromEntity($entity) {
    $items = field_get_items($this->entityType, $entity, $this->imageField);
    if (!empty($items)) {
      $ofv = field_view_value($this->entityType, $entity, $this->imageField, $items[0], array());
      $field_value = !empty($ofv['#item']) ? $ofv['#item']['fid'] : NULL;
    }
    else {
      $field_value = NULL;
    }

    return $field_value;
  }


  /**
   * Sets $this->orig_fid to the original file id if one exists.
   *
   * If there is not an original entity, NULL will be set.
   */
  protected function setOrigFid() {
    if (isset($this->entity->original) && !empty($this->entity->original)) {
      $orig_fid = $this->getFidFromEntity($this->entity->original);
    }
    else {
      $orig_fid = NULL;
    }

    $this->origFid = $orig_fid;
  }

  /**
   * Sets $this->new_fid from the entity.
   */
  protected function setNewFid() {
    $this->newFid = $this->getFidFromEntity($this->entity);
  }
}
