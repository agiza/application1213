<?php

/**
 * @file media_s3/includes/MediaInterenetS3Handler.inc
 *
 * Contains MediaInternetS3Handler.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetS3Handler extends MediaInternetBaseHandler {
  /**
   * Check if a S3 video id is valid.
   *
   * Check against the oembed stream instead of the gdata api site to
   * avoid "yt:quota too_many_recent_calls" errors.
   *
   * @return
   *   Boolean.
   */
  static public function validId($id) {
    
    // NOT SUPPORTED YET
    
    return TRUE;
  }

  public function parse($embedCode) {
    //replace spaces with %20 in URL
    $embedCode = str_replace(' ', '%20', $embedCode);
    
    $patterns = array(
      '@([^"\& ]+)\.s3\.amazonaws\.com/([^"\& ]+)@i',
    );
    
    foreach ($patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      
       $bucket = explode('://', $matches[1]);
       
      // @TODO: Parse is called often. Refactor so that valid ID is checked
      // when a video is added, but not every time the embedCode is parsed.
      // if (isset($matches[1]) && self::validId($matches[1])) {
      if (isset($matches[2])) {
        return file_stream_wrapper_uri_normalize('media-s3://b/' . $bucket[1] . '/v/' . $matches[2]);
      }
    }
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);

    if (empty($file->fid) && $info = $this->getOEmbed()) {
      $file->filename = truncate_utf8($info['title'], 255);
    }

    return $file;
  }

  /**
   * Returns information about the media. See http://video.search.yahoo.com/mrss.
   *
   * @return
   *   If ATOM+MRSS information is available, a SimpleXML element containing
   *   ATOM and MRSS elements, as per those respective specifications.
   *
   * @todo Would be better for the return value to be an array rather than a
   *   SimpleXML element, but media_retrieve_xml() needs to be upgraded to
   *   handle namespaces first.
   */
  public function getMRSS() {
    
    // NOT SUPPORTED YET
    
  }

  /**
   * Returns information about the media. See http://www.oembed.com/.
   *
   * @return
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    
    // NOT SUPPORTED YET
    
  }
}
