from setuptools import find_packages, setup

package_name = 'grbl_bridge'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(include=[package_name, f'{package_name}.*']),
    data_files=[
        ('share/ament_index/resource_index/packages',
         [f'resource/{package_name}']),
        (f'share/{package_name}', ['package.xml']),
    ],
    install_requires=['setuptools', 'pyserial'],
    zip_safe=True,
    maintainer='ExArm Robotics',
    maintainer_email='dev@exarm.local',
    description='Serial bridge between ROS 2 and grblHAL.',
    license='Apache License 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'grbl_bridge_node = grbl_bridge.grbl_bridge_node:main',
        ],
    },
)
