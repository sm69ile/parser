pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                echo 'Building...'
		sh 'autoreconf --install && ./configure --prefix=/usr/local/software/jenkins && make && make install'
            }
        }
    }
}