var exampleController = angular.module("exampleController",["firebase"]);

exampleController.controller("LoginController", ["$scope","$location", function MyController($scope, $location){
	$scope.login = function(){
		$location.path("/dashboard");
	};

	$scope.register = function(){
		$location.path("/dashboard");
	};
}]);
	
exampleController.controller("DashboardController", ["$scope", "$firebase", function MyController($scope, $firebase){

	var firebaseRef = new Firebase("https://flickering-fire-7979.firebaseio.com/dashboard");


	var firebaseContent = $firebase(firebaseRef);

	console.log(firebaseContent.$asObject());


	$scope.firebaseContent = firebaseContent.$asObject();
}]);

exampleController.controller("ListController", ["$scope", "$http", function MyController($scope, $http){
	$http.get("js/example.json").success(function(data){
		$scope.choices = data;
	});
}]);

exampleController.controller("DetailsController", ["$scope", "$http", "$routeParams", function MyController($scope, $http, $routeParams){
	$http.get("js/example.json").success(function(data){
		$scope.choices = data;
		$scope.whichChoice = $routeParams.itemInstruction;

		if ($routeParams.itemInstruction > 0) {
			$scope.prevItem = Number($routeParams.itemInstruction) - 1;
		} else {
			$scope.prevItem = $scope.choices.length - 1;
		}

		if ($routeParams.itemInstruction < ($scope.choices.length - 1)) {
			$scope.nextItem = Number($routeParams.itemInstruction) + 1;
		} else {
			$scope.nextItem = 0;
		}
	});
}]);