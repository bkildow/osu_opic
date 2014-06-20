<?php
/**
 * @file
 * Manages and coordinates opic and file states.
 */

/**
 * Class OsuOpicStateManager
 */
class OsuOpicStateManager {
  protected $fs;
  protected $os;
  protected $fileState;
  protected $opicState;
  protected $entity;

  /**
   * Constructor.
   *
   * @param string $entity_type
   *   drupal entity type
   * @param StdClass $entity
   *   drupal entity
   * @param string $image_field
   *   image field name
   */
  public function __construct($entity_type, $entity, $image_field) {
    $this->fs = new OsuOpicFileState($entity_type, $entity, $image_field);
    list($eid, ,) = entity_extract_ids($entity_type, $entity);
    $this->os = new OsuOpicOpicState($entity_type, $eid);

    $this->fileState = $this->getFileState();
    $this->opicState = $this->getOpicState();
    $this->entity = $entity;

  }

  /**
   * Delegator to get the current file state from OsuOpicFileState.
   *
   * @return int
   *   a file state as defined by OsuOpicFileState
   */
  public function getFileState() {
    return $this->fs->getState();
  }

  /**
   * Delegator to get the current opic state from OsuOpicOpicState.
   *
   * @return int
   *   an opic state as defined by OsuOpicOpicState
   */
  public function getOpicState() {
    return $this->os->getState();
  }

  /**
   * Set the opic state, as long as this isn't a new entity.
   *
   * If it is new, there won't be an entity id to save, and this will fail.
   *
   * @param int $state
   *   state as defined by OsuOpicOpicState
   */
  public function setOpicState($state) {
    if (!$this->entity->is_new) {
      $this->os->setState($state);
    }
  }

  /**
   * Checks various states to see if image push is needed.
   *
   * @return bool
   *   true if this is needed
   */
  public function imagePushNeeded() {

    if ($this->fileState == OsuOpicFileState::IMAGE_NEW && $this->opicState !== OsuOpicOpicState::OPIC_LOCAL) {
      return TRUE;
    }

    if ($this->fileState == OsuOpicFileState::IMAGE_EXISTS_UNCHANGED && $this->opicState == OsuOpicOpicState::OPIC_NULL) {
      return TRUE;
    }

    if ($this->fileState == OsuOpicFileState::IMAGE_UPDATED && $this->opicState !== OsuOpicOpicState::OPIC_LOCAL) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Checks various states to see if image pull is needed.
   *
   * @return bool
   *   true if this is needed
   */
  public function imagePullNeeded() {

    // Figure out if this is needed, by seeing if the others are not.
    $push = $this->imagePushNeeded();
    $local = $this->imageLocalNeeded();

    if (!$push && !$local) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Checks various states to see if image local is needed.
   *
   * @return bool
   *   true if this is needed
   */
  public function imageLocalNeeded() {
    if ($this->opicState == OsuOpicOpicState::OPIC_LOCAL && $this->fileState !== OsuOpicFileState::IMAGE_DELETED) {
      return TRUE;
    }

    if ($this->opicState == OsuOpicOpicState::OPIC_LOCAL && $this->fileState == OsuOpicFileState::IMAGE_UPDATED) {
      return TRUE;
    }

    return FALSE;
  }

}
