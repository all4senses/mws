(function ($) {

  Drupal.behaviors.checkout_prompt = {
    attach: function (context, settings) {
       
       $(".checkout_to_original").click(function(event){
         var quit = prompt('checkout_to_original?');
         if (!quit) {
          //alert('checkout_to_original');
          event.preventDefault();
         }
       });

       
    }
  };

}(jQuery));
