pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                echo 'Building...'
		sh 'autoreconf --install && CFLAGS=-I/usr/local/include LDFLAGS=-L/usr/local/lib ./configure --prefix=/usr/local/software/jenkins && make && make install'
            }
        }
    }
}