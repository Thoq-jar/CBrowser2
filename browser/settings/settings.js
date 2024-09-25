$(document).ready(() => {
    $('.tab').click(function() {
        const contentId = $(this).data('content');
        $('.content').hide();
        $('#' + contentId).show();
        $('.tab').removeClass('active');
        $(this).addClass('active');
    });

    $('#search-bar').on('input', function() {
        const searchTerm = $(this).val().toLowerCase();
        $('.content').each(function() {
            const isVisible = $(this).text().toLowerCase().includes(searchTerm);
            $(this).toggle(isVisible);
        });
    });
});
