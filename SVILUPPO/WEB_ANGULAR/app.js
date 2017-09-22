angular.module('domotica', ['ui.bootstrap','ui.router','ngAnimate']);

angular.module('domotica').config(function($stateProvider, $urlRouterProvider) {

    $stateProvider.state('home', {
            url: '/home',
            views: {
                '': {templateUrl: 'partial/home/home.html'},
                'header@home': {templateUrl: "partial/header/header.html"},
                'footer@home': {templateUrl: "partial/footer/footer.html"}
            }
        });
        /* Add New States Above */
        $urlRouterProvider.otherwise('/home');

});

angular.module('domotica').run(function($rootScope) {

    $rootScope.safeApply = function(fn) {
        var phase = $rootScope.$$phase;
        if (phase === '$apply' || phase === '$digest') {
            if (fn && (typeof(fn) === 'function')) {
                fn();
            }
        } else {
            this.$apply(fn);
        }
    };

});
