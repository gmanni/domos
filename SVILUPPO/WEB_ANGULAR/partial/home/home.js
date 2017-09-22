angular.module('domotica')
	.controller('HomeCtrl',['$scope','$http','$log', function($scope, $http, $log){
        $scope.config = [];

	    var request = $http({
            // headers: 'Content-Type': 'application/html',
            url: "../../data/config.json",
            method: 'GET',
            cache: false,
            transformResponse: function (data, headersGetter, status) {
                //This was implemented since the REST service is returning a plain/text response
                //and angularJS $http module can't parse the response like that.
                return {data: angular.fromJson(data)};
            }
            }).success(function (data, status, headers, config) {
                $log.debug("loading config.json SUCCESS");
                $scope.config = data.data;
                $log.debug($scope.config);
               
                // controllo se c'è almeno un pin configurato
                $scope.almenoUnoConfigurato = false;
                for(var i=0; i< $scope.config.digital.length; i++){
                    $log.debug((i+1) +  " " + $scope.config.digital[i].type);
                    if($scope.config.digital[i].assigned !== ""){
                        $scope.almenoUnoConfigurato = true;
                        break;
                    }
                }

            }).error(function (data, status, headers, config) {
                $log.error('errore = ', data);
            });
	}]);