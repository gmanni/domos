angular.module('domotica')
	.controller('HomeCtrl',['$scope','$http','$log', function($scope, $http, $log){
		$scope.config = []; // Initialize with an empty array
	    var request = $http({
            // headers: 'Content-Type': 'application/html',
            url: "config.txt",
            method: 'GET',
            cache: false,
            transformResponse: function (data, headersGetter, status) {
                //This was implemented since the REST service is returning a plain/text response
                //and angularJS $http module can't parse the response like that.
                return {data: angular.fromJson(data)};
            }
            }).success(function (data, status, headers, config) {
                $log.debug("SUCCESS");
                $log.debug(data);
               
            })
            .error(function (data, status, headers, config) {
                $log.error('errore = ', data);
            });
	}]);