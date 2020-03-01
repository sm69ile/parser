pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                echo 'Building...'
		sh 'autoreconf --install && ./configure --prefix=/tmp && make && make install'
            }
        }
    }
}