#include "n_body.cpp"

#include <vector>
#include <string>

#define VERSION     121225

using namespace std;

void start_simulation(n_body &_world)
{
    vector< string > simulation_options;
    simulation_options.push_back("Generate randomly");
    simulation_options.push_back("Manually type in them");
    simulation_options.push_back("Import from file");
    simulation_options.push_back("Return");

    menu simulation_menu("How will the initial condition be defined?", simulation_options);
    simulation_menu.print();

    switch(simulation_menu.getSelection())
    {
    case 1:
        _world.generate(1);
        break;
    case 2:
        _world.generate(2);
        break;
    case 3:
        _world.generate(3);
        break;
    }

    _world.printResult();
    _world.calculate();
    _world.printResult();

    //TODO: pause at here
    system("pause");
}

void time_dependent_factor(n_body &_world)
{
    vector< string > time_dependent_factor_options;
    time_dependent_factor_options.push_back("Total execution time");
    time_dependent_factor_options.push_back("Single step duration");
    time_dependent_factor_options.push_back("Reset to default");
    time_dependent_factor_options.push_back("Return");

    menu time_dependent_factor_menu("Time dependent factors",
                                    time_dependent_factor_options);
    menu acquire_var;
    time_dependent_factor_menu.setPrecision(2);

    bool repeat = true;
    do
    {
        time_dependent_factor_menu.printVariable("Total execution time",
                _world.getSumT());
        time_dependent_factor_menu.printVariable("Single step duration",
                _world.getDeltaT());

        time_dependent_factor_menu.print();
        switch(time_dependent_factor_menu.getSelection())
        {
        case 1:
            acquire_var.setReload("Enter TOTAL EXECUTION TIME: ");
            _world.setSumT(acquire_var.getConstant());
            break;
        case 2:
            acquire_var.setReload("Enter SINGLE STEP DURATION: ");
            _world.setDeltaT(acquire_var.getConstant());
            break;
        case 3:
            break;
        case 4:
            repeat = false;
            break;
        }
    }
    while(repeat);
}

void simulation_properties(n_body &_world)
{
    vector< string > simulation_properties_options;
    simulation_properties_options.push_back("Mass boundaries");
    simulation_properties_options.push_back("Position boundaries");
    simulation_properties_options.push_back("Velocity boundaries");
    simulation_properties_options.push_back("Time dependent factors");
    simulation_properties_options.push_back("Gravity constant");
    simulation_properties_options.push_back("Reset to default");
    simulation_properties_options.push_back("Return");

    menu simulation_properties_menu("Simulation Properties",
                                    simulation_properties_options);
    menu acquire_range;
    simulation_properties_menu.setPrecision(2);

    Boundary newBoundary = _world.getBoundary();
    DigitSet tempRange;

    bool repeat = true;
    do
    {
        simulation_properties_menu.printVariable("Mass boundaries",
                newBoundary.massLower, newBoundary.massUpper);
        simulation_properties_menu.printVariable("Position boundaries",
                newBoundary.positionLower, newBoundary.positionUpper);
        simulation_properties_menu.printVariable("Velocity boundaries",
                newBoundary.velocityLower, newBoundary.velocityUpper);
        simulation_properties_menu.printVariable("Gravity constant",
                _world.getGravity());

        simulation_properties_menu.print();
        switch(simulation_properties_menu.getSelection())
        {
        case 1:
            acquire_range.setReload("Enter MASS BOUNDARIES:(min/max)");
            tempRange = acquire_range.getRange();
            newBoundary.massLower = tempRange.x;
            newBoundary.massUpper = tempRange.y;
            break;
        case 2:
            acquire_range.setReload("Enter POSITION BOUNDARIES:(min/max)");
            tempRange = acquire_range.getRange();
            newBoundary.positionLower = tempRange.x;
            newBoundary.positionUpper = tempRange.y;
            break;
        case 3:
            acquire_range.setReload("Enter VELOCITY BOUNDARIES:(min/max)");
            tempRange = acquire_range.getRange();
            newBoundary.velocityLower = tempRange.x;
            newBoundary.velocityUpper = tempRange.y;
            break;
        case 4:
            time_dependent_factor(_world);
            break;
        case 5:
            acquire_range.setReload("Enter GRAVITY CONSTANT:");
            _world.setGravity(acquire_range.getConstant());
            break;
        case 6:
            break;
        case 7:
            repeat = false;
            break;
        }
    }
    while(repeat);

    _world.setBoundary(newBoundary);
}

void plot_configurations()
{

}

int main()
{
    vector< string > frontier_options;
    frontier_options.push_back("Start simulation");
    frontier_options.push_back("Edit simulation properties");
    frontier_options.push_back("Edit plot configurations");
    frontier_options.push_back("Exit program");

    menu frontier_menu(VERSION, frontier_options);

    n_body world;

    bool repeat = true;
    do
    {
        frontier_menu.print();
        switch(frontier_menu.getSelection())
        {
        case 1:
            start_simulation(world);
            break;
        case 2:
            simulation_properties(world);
            break;
        case 3:
            plot_configurations();
            break;
        case 4:
            repeat = false;
            break;
        }
    }
    while(repeat);

    return 0;
}
