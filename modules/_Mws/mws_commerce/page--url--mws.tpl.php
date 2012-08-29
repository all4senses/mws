<div id="mws" class="clearfix">

   <header id="header" role="banner" class="clearfix">

    <?php print render($page['header']); ?>
      <nav id="navigation" role="navigation" class="clearfix">
      </nav> <!-- /#navigation -->
    <?php //if ($breadcrumb): print $breadcrumb; endif;?>
  </header> <!-- /#header -->

  <section id="main-mws" role="main" class="clearfix">
    <?php print $messages; ?>
    <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
    <?php if (!empty($tabs['#primary'])): ?><div class="tabs-wrapper clearfix"><?php print render($tabs); ?></div><?php endif; ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    <?php print render($page['content']); ?>
  </section> <!-- /#main -->
  

  <footer id="footer" role="contentinfo" class="clearfix">
    <?php print render($page['footer']) ?>
  </footer> <!-- /#footer -->

</div> <!-- /#container --> 
