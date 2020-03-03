pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                echo 'Building...'
		sh 'autoreconf --install && ./configure --prefix=/usr/local/jenkins && make && make install'
            }
        }
    }
}