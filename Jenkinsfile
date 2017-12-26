pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                echo 'Building..'
		sh 'make'
		archiveArtifacts artifacts: '**/Semantikin', fingerprint: true
            }
        }
        stage('Test') {
            steps {
                echo 'Testing..'
		sh 'make tests'
            }
        }
        stage('Deploy') {
            steps {
		steps {
			echo 'Deploying....'
			sh 'cp Semantiking ~/'
		}
            }
        }
    }
}
