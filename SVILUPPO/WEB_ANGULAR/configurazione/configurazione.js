angular.module('configurazione', ['ui.bootstrap', 'ui.router','ngAnimate']);

angular.module('configurazione').config(function($stateProvider) {

    /* Add New States Above */
	$stateProvider.state('elenco', {
        url: '/elenco',
        views:{
            '': {templateUrl: 'partial/home/home.html'},
            'corpo@elenco': {templateUrl: 'configurazione/partial/elenco/elenco.html'},
            'header@elenco': {templateUrl: "partial/header/header.html"},
            'footer@elenco': {templateUrl: "partial/footer/footer.html"}
        }
        
    });
});

