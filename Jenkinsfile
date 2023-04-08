pipeline {
    agent none
    stages {
        stage('All') {
            matrix {
                agent {
                    label "${platform}"
                }
                axes {
                    axis {
                        name 'platform'
                        values 'ubuntu22.04-x86_64','ubuntu20.04-x86_64', 'ubuntu18.04-x86_64', 'windows10-x64', 'windows10-x86', 'raspbian10-armv7l', 'rocky8-x86_64'
                    }
                }
                stages {
                    stage('Build') {
                        steps {
                            cmakeBuild buildDir: 'build', cleanBuild: true, installation: 'InSearchPath', buildType: 'Release', cmakeArgs: '-G Ninja -DCDEPLOY_DEBUG_BUILD=ON'
                            cmake workingDir: 'build', arguments: '--build . --target package', installation: 'InSearchPath'
                            dir('build') {
                                archiveArtifacts artifacts: 'mingtest-*.zip'
                            }
                        }
                    }
                    stage('Test') {
                        steps {
                            ctest workingDir: 'build', installation: 'InSearchPath', arguments: '--output-on-failure'
                        }
                    }
                }
            }
        }
    }
}

