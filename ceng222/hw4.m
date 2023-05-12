
N=4147;   
lambda_auto = 50;  
lambda_truck = 10; 

TotalWeight =  zeros(N,1);

for k=1:N;

	truckRandom = rand; j = 0; 
	FforTruck = exp(-lambda_truck);
	while (truckRandom >= FforTruck);
		j=j+1;
		FforTruck = FforTruck + exp(-lambda_truck) * lambda_truck^j / gamma(j+1);
	end;
	WeightTruck = (sum( -100 * log(rand(110,j))));
	TotalTruck = sum(WeightTruck);

	CarRandom= rand; i = 0; 
	FforAuto = exp(-lambda_auto);
	while (CarRandom >= FforAuto);
		i=i+1;
		FforAuto = FforAuto + exp(-lambda_auto) * lambda_auto^i/gamma(i+1);
	end;
	WeightAuto =(sum(-1/0.15 * log(rand(190,i))));
	TotalAuto = sum(WeightAuto);
	

	TotalWeight(k) = TotalTruck+TotalAuto;
	
end;
standardWeight = std(TotalWeight);
total = mean(TotalWeight);
probability =mean(TotalWeight>200000);

fprintf('probability that the total weight of all the vehicles that pass over the bridge on a day = %f\n',probability);

fprintf('Estimation of the total weight = %f\n',total);

fprintf('Std(X) = %f\n',standardWeight);


