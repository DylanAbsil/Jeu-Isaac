pipeline {
  agent any
  stages {
    stage('Checkout') {
      steps {
        echo 'git Checkout'
      }
    }
    stage('Syntax ') {
      steps {
        echo 'Checking syntax'
      }
    }
    stage('Build') {
      steps {
        echo 'Building iar script'
      }
    }
    stage('Result') {
      steps {
        echo 'Checking result, parsing error etc'
      }
    }
    stage('Deploy') {
      steps {
        echo 'Deploiement de la solution'
      }
    }
  }
}