# tplate
A simple project templating program

This project  is made to automate the creation of basic C projects utilising a 
CMake build setup. It can create simple executable projects ready for
the addition of dependencies, as well as shared and static library projects
that come pre-configured to work with standard CMake packaging.
Additionally, it can produce library projects that produce both static
and shared libraries at the same time, enabling the use of either in dependent
projects.



## Getting Started

These instructions will get you a copy of the project up and running
on your local machine for development and testing purposes. See
the Installing section for notes on how to deploy the project on a
live system.

### Prerequisites

You will need to have the required libraries installed on your
machine. These projects can be found at these links:
* [Melephas/list](https://github.com/Melephas/list)
* [Melephas/dstring](https://github.com/Melephas/dstring)

### Installing

After downloading the project file and installing the required
libraries navigate to the project root directory and run the
following commands:
```shell
$ cmake -B build
$ cd build
$ make
$ sudo make install
```

## Built With

* [Melephas/list](https://github.com/Melephas/list) - Type erasure
based linked list
* [Melephas/dstring](https://github.com/Melephas/dstring) - 
Dynamically allocated and managed string

## Contributing

Please read 
[CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426)
for details on our code of conduct, and the process for submitting
pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions
available, see the 
[tags on this repository](https://github.com/Melephas/tplate/tags).

## Authors

* **Sam Miller** - *Initial work* - 
[Melephas](https://github.com/Melephas)

See also the list of 
[contributors](https://github.com/Melephas/tplate/graphs/contributors)
who participated in this project.

## License

This project is licensed under the AGPLv3 License - see the
[LICENSE](LICENSE) file for details

