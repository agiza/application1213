<?php

/**
 * @file media_s3/includes/MediaS3Browser.inc
 *
 * Definition of MediaS3Browser.
 */

/**
 * Media browser plugin for displaying a specific view and display.
 */
class MediaS3Browser extends MediaBrowserPlugin {
  /**
   * Implements MediaBrowserPluginInterface::access().
   */
  public function access($account = NULL) {
    // @TODO: media_access() is a wrapper for file_entity_access(). Switch to the
    // new function when Media 1.x is deprecated.
    return media_access('create', $account);
  }

  /**
   * Implements MediaBrowserPlugin::view().
   */
  public function view() {
    // Check if the user is able to add remote media.
    if (user_access('add media from remote sources')) {
      $build['form'] = drupal_get_form('media_s3_add', $this->params['types'], $this->params['multiselect']);
      return $build;
    }
  }
}
