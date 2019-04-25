pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                echo 'Building..'
		sh "autoreconf --install && configure && make"
            }
        }
    }
}