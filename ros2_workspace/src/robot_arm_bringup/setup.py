from setuptools import find_packages, setup

package_name = 'robot_arm_bringup'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(include=[package_name, f'{package_name}.*']),
    data_files=[
        ('share/ament_index/resource_index/packages',
         [f'resource/{package_name}']),
        (f'share/{package_name}/launch', [
            'launch/display.launch.py',
            'launch/sweep_demo.launch.py',
        ]),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='ExArm Robotics',
    maintainer_email='dev@exarm.local',
    description='Launch descriptions and test nodes for the ExArm two-DoF prototype.',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'test_sweep = robot_arm_bringup.test_sweep:main',
        ],
    },
)
