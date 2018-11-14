$(document).ready(function(){
    $('.nextbook').click(function() {
        window.brcore.navNextBook();
    });
    $('.prevbook').click(function() {
        window.brcore.navPrevBook();
    });

    $('.nextchapter').click(function() {
        window.brcore.navNextChapter();
    });
    $('.prevchapter').click(function() {
        window.brcore.navPrevChapter();
    });

    // toggle bible versions in comparing browser
    $('button.bible').click(function() {
        if ($(this).hasClass('active')) {
            // remove active class and remove all this bible version's verses
            $(this).removeClass('active');
            $('th[value='+$(this).val()+']').hide();
            $('td[value='+$(this).val()+']').hide();
        } else {
            $(this).addClass('active');
            $('th[value='+$(this).val()+']').show();
            $('td[value='+$(this).val()+']').show();
        }
    });

    // show title
    $('h2#bookname').text(window.brCore.getCurrentBook().getLongName());

    // build compare browser verse table
    var bibles = window.brCore.getAllBibleVersions();
    for(var i=0; i < bibles.length; i++) {

    }
});
