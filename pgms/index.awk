BEGIN	{ 
	FS="|" ; 
	sum=0.00; 
	n=0;
	printf("\n                     INDEX VALUES            \n");
	printf("TEST%40sBASELINE     RESULT      INDEX\n\n","");

	}

	{ # process all lines - use geometric means
	idex = $10/$5;
	sum += idex;
	++n;
	printf("%-40s  %10.1f %10.1f %10.1f\n",$1,$5,$10,idex);
	}

END	{
	printf("              %30s                     =========\n","");
	printf("     SUM of %2d items  %30s  %20.1f\n",n,"",sum);
	printf("     AVERAGE          %30s  %20.1f\n","",sum/n);
	}
