<div id="mws" class="clearfix">

   <header id="header-mws" role="banner" class="clearfix">

    <?php print render($page['header']); ?>
      <nav id="navigation" role="navigation" class="clearfix">
        <?php if (isset($mws_header)) { print render($mws_header); } ?>
      </nav> <!-- /#navigation -->
    <?php //if ($breadcrumb): print $breadcrumb; endif;?>
  </header> <!-- /#header -->

  <section id="main-mws" role="main" class="clearfix">
    <?php print $messages; ?>
    <?php if ($title): ?><div class="title" id="page-title"><?php print $title; ?></div><?php endif; ?>
    <?php if (!empty($tabs['#primary'])): ?><div class="tabs-wrapper clearfix"><?php print render($tabs); ?></div><?php endif; ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    <?php print render($page['content']); ?>
  </section> <!-- /#main -->
  
  <footer id="footer-mws" role="contentinfo" class="clearfix">
  </footer> <!-- /#footer -->

</div> <!-- /#container --> 
