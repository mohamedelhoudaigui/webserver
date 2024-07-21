/* script.js */

document.getElementById('testButton').addEventListener('click', function() {
    document.getElementById('message').textContent = 'Button clicked!';
});

document.getElementById('testForm').addEventListener('submit', function(event) {
    event.preventDefault();
    const formData = new FormData(event.target);

    fetch('/submit', {
        method: 'POST',
        body: formData
    })
    .then(response => response.text())
    .then(data => {
        document.getElementById('message').textContent = data;
    })
    .catch(error => {
        console.error('Error:', error);
        document.getElementById('message').textContent = 'An error occurred';
    });
});
