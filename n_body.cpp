#include "n_body.h"

n_body::n_body()
{
    //Default boudaries, prevent unpredictable result
    boundary.positionUpper = 100;
    boundary.positionLower = -100;
    boundary.velocityLower = -10;
    boundary.velocityUpper = 10;
    boundary.massUpper = 50;
    boundary.massLower = 0;

    gravity = DEFAULT_GRAVITY;
    sumT = DEFAULT_SUMT;
    deltaT = DEFAULT_DELTAT;
}

void n_body::calculate(void)
{
    for(float i = 0; i < sumT; i += deltaT)
    {
        for(int index1 = 0; index1 < data.size(); index1++)
        {
            for(int index2 = 0; index2 < data.size(); index2++)
            {
                if(index1 != index2)
                {
                    data[index1].reciprocation(data[index2]);
                }
            }
        }
        for(int index = 0; index < data.size(); index++)
            data[index].updatePosition();
    }
}

void n_body::byRandom()
{
    random generateRandom;
    for(int i = 0; i < amount; i ++)
    {
        particle temporaryParticle(generateRandom, toString( i+1 ), boundary);
        data.push_back(temporaryParticle);
    }
}

void n_body::byManual()
{
    menu acquire_var;

    float mass;
    DigitSet position;
    DigitSet velocity;

    acquire_var.printQuestion("Manual initial condition setup");

    for(int i = 0; i < amount; i ++)
    {
        cout << " " << "Star " << toString( i+1 ) << endl;

        acquire_var.setReload("-Mass:");
        mass = acquire_var.getConstant(false);

        acquire_var.setReload("-Position:(x,y)");
        position = acquire_var.getRange(false);

        acquire_var.setReload("-Velocity:(x,y)");
        velocity = acquire_var.getRange(true);
    }
}

void n_body::byFile()
{
    file input;
    data = input.getData();
}

void n_body::getAmount(void)
{
    bool status;

    do
    {
        cout << " How many stars to simulate? ";
        while(!(cin >> amount))
        {
            cout << "  * Not an integer." << endl << endl;
            cout << " How many stars to simulate? ";

            cin.clear();
            cin.ignore(1000, '\n');
        }

        status = true;
        if(amount < 0)
            cout << "  * Quantity can't be negative." << endl;
        else if(amount > MAX_SIMU_AMOUNT)
            cout << "  * Simulation amount out of range." << endl;
        else
            status = false;
        cout << endl;
    }
    while(status);
}
