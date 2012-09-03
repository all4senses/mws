<?php

/**
 * @file
 * Generates taxonomy page keyword report on node edit form
 */

/**
 * AJAX handler to refresh tax report
 */
function kwresearch_keywords_tax_report_js() {

  $output = array();
  $form = kwresearch_keyword_tax_report_form($_POST['keywords'], $_POST['vid']);
  $output['report']['output'] = drupal_render($form);
  $output['inputs']['vid'] = $_POST['vid'];

  drupal_json_output($output);
}

/**
 * Generates tax report form api field
 *
 * @param str|array $keywords - list of keywords to include in report as CSV or array of keywords
 * @param int $vid - vocabulary id
 */
function kwresearch_keyword_tax_report_form($keywords, $vid) {
  $form['kwresearch-tax-report-' . $vid] = array(
    '#type' => 'item',
    '#title' => t('Keyword report'),
    '#markup' => kwresearch_keywords_tax_report($keywords),
    '#prefix' => '<div id="kwresearch-tax-report-' . $vid . '" class="kwresearch-tax-report kwresearch-tax-report-' . $vid . '">',
    '#suffix' => '</div>',
    '#weight' => $weight++,
  );
  return $form;
}

/**
 * Generates report for taxonomies
 *
 * @param str|array $keywords - list of keywords to include in report as CSV or array of keywords
 */
function kwresearch_keywords_tax_report($keywords) {

  $keywords = strtolower($keywords);
  if (!is_array($keywords)) {
    $keywords = explode(',', $keywords);
  }
  //$kws = '';

  $kws_a = array();
  $processed = array();
  foreach ($keywords as $keyword) {
    $keyword = trim($keyword);
    if (!$keyword) {
      continue;
    }
    $processed[$keyword] = 0;
    //$kws .= (($kws)?',':'') . '"' . $keyword . '"'; 

    $kws_a[] = $keyword;
  }
  $header = array(
    array(
      'data' => t('Keyword'),
      'field' => 'k.keyword',
    ),
    array(
      'data' => t('Page priority'),
      'field' => 'pk.priority',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Set by'),
      'field' => 'up.name',
    ),
    array(
      'data' => t('Site priority'),
      'field' => 'k.priority',
    ),
    array(
      'data' => t('Value'),
      'field' => 'k.value',
    ),
    array(
      'data' => t('Set by'),
      'field' => 'u.name',
    ),
    array(
      'data' => t('Pages'),
      'field' => 'k.page_count',
    ),
    array(
      'data' => t('Total Daily'),
      'field' => 'k.daily_volume',
    ),
    array('data' => t('Operations')),
  );

  if ($kws_a) {
    //$kws_a = explode(',', $kws);

    $placeholders = implode(',', array_fill(0, count($kws_a), '"%s"'));
    $sql = "
      SELECT DISTINCT k.kid, pk.priority AS page_priority, pk.uid AS page_uid, k.*, u.name AS username, up.name AS page_username
      FROM {kwresearch_keyword} k
      LEFT JOIN {kwresearch_page_keyword} pk ON pk.kid = k.kid
      LEFT JOIN {users} u ON u.uid = k.uid
      LEFT JOIN {users} up ON up.uid = pk.uid
      WHERE k.keyword IN ($placeholders)
    ";
    $tablesort = tablesort_sql($header);
    $sql = $sql . $tablesort;
    $result = pager_query($sql, 100, 0, NULL, $kws_a);

    $priorities = kwresearch_get_priority_options();
    while ($r = db_fetch_object($result)) {
      $processed[$r->keyword] = 1;
      $rows[] = array(
        'data' => array(
          // Cells
          check_plain($r->keyword),
          ($r->page_priority > 0) ? $priorities[$r->page_priority] : '-',
          ($r->page_username == NULL) ? '-' : l($r->page_username, 'user/' . $r->page_uid),
          ($r->priority > 0) ? $priorities[$r->priority] : '-',
          ($r->value >= 0) ?  t('$') . number_format($r->value, 2) : '-',
          ($r->username == NULL) ? '-' : l($r->username, 'user/' . $r->uid),
          ($r->page_count == NULL) ? '-' : l(number_format($r->page_count), 'admin/structure/kwresearch/keyword_pages/' . $r->kid, array('target' => '_blank')),
          ($r->daily_volume == NULL) ? '-' : number_format($r->daily_volume),
          l(t('stats'), 'admin/structure/kwresearch/keyword_report/' . $r->keyword, array('attributes' => array('target' => '_blank'))) . ' | ' .
          l(t('pages'), 'admin/structure/kwresearch/keyword_pages/' . $r->kid, array('attributes' => array('target' => '_blank'))) 
          ,
        ),
        // Attributes for tr
        'class' => "kwresearch",
      );
    }
    foreach ($processed as $keyword => $v) {
      if (!$v) {
        $rows[] = array(
          'data' => array(
            // Cells
            check_plain($keyword),
            t('No data'),
            '',
            '',
            '',
            '',
            '',
            '',
            l(t('stats'), 'admin/structure/kwresearch/keyword_report/' . $keyword, array('attributes' => array('target' => '_blank'))),
          ),
          // Attributes for tr
          'class' => "kwresearch",
        );
      }
    }
  }
  if (!$rows) {
    $rows[] = array(array(
        'data' => t('No keywords were found.'),
        'colspan' => count($header),
      ));
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'kwresearch-tax-keywords')));
  //$output .= theme('pager', NULL, 100, 0);


  return $output;
}
