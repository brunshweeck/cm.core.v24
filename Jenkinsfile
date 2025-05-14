pipeline {
    agent any

    environment {
        BUILD_DIR = "build"
    }

    stages {
        stage('Checkout') {
            steps {
                checkout scm  // Récupère le code depuis Git
            }
        }

        stage('Configure CMake') {
            steps {
                sh """
                    mkdir -p ${BUILD_DIR}
                    cd ${BUILD_DIR}
                    cmake -G Ninja ..
                """
            }
        }

        stage('Build with Ninja') {
            steps {
                sh """
                    cd ${BUILD_DIR}
                    ninja
                """
            }
        }

        stage('Run Tests (Optional)') {
            steps {
                sh """
                    cd ${BUILD_DIR}
                    ctest --output-on-failure
                """
            }
        }
    }

    post {
        always {
            cleanWs()  // Nettoyer l'espace de travail
        }
    }
}
