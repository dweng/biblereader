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
});
