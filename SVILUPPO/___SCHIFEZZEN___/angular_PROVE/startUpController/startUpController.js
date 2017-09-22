var myAppModule = angular.module('myApp', []);

myAppModule.controller('startUpController', function($scope) {
	$scope.funding = { startingEstimate: 0 };
	
	computeNeeded = function() {
		$scope.funding.needed = $scope.funding.startingEstimate * 10;
	};
	
	// questa chiamata serve a fare l'update sulla view
	$scope.$watch('funding.startingEstimate', computeNeeded);

});