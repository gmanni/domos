var myAppModule = angular.module('myApp', []);

myAppModule.controller('textController', function($scope) {
	var someText = {};
	someText.message = 'Ciao, come stai?';
	$scope.someText = someText;
});
