// Show/Hide Forms
document.getElementById('show-sign-up').addEventListener('click', function () {
    document.getElementById('sign-up-form').classList.remove('hidden');
    document.getElementById('sign-in-form').classList.add('hidden');
});

document.getElementById('show-sign-in').addEventListener('click', function () {
    document.getElementById('sign-in-form').classList.remove('hidden');
    document.getElementById('sign-up-form').classList.add('hidden');
});

// Email Validation Function
function validateEmail(email) {
    const regex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/; // Standard email regex
    return regex.test(email);
}

// RFID Validation Function
function validateRFID(rfid) {
    return rfid.startsWith('RFID'); // Checks if RFID starts with 'RFID'
}

// Event Listener for Email Validation
document.getElementById('email').addEventListener('input', function () {
    const email = this.value;
    const errorSpan = document.getElementById('signup-email-error');
    if (!validateEmail(email)) {
        errorSpan.textContent = 'Invalid email format. Please include "@" and a domain.';
    } else {
        errorSpan.textContent = '';
    }
});

document.getElementById('login-email').addEventListener('input', function () {
    const email = this.value;
    const errorSpan = document.getElementById('signin-email-error');
    if (!validateEmail(email)) {
        errorSpan.textContent = 'Invalid email format. Please include "@" and a domain.';
    } else {
        errorSpan.textContent = '';
    }
});

// Event Listener for RFID Validation
document.getElementById('rfid').addEventListener('input', function () {
    const rfid = this.value;
    const errorSpan = document.getElementById('rfid-error');
    if (!validateRFID(rfid)) {
        errorSpan.textContent = 'Invalid RFID. It must start with "RFID".';
    } else {
        errorSpan.textContent = '';
    }
});

// Prevent Form Submission if RFID or Email is Invalid
document.getElementById('sign-up-form').addEventListener('submit', function (e) {
    const rfid = document.getElementById('rfid').value;
    const email = document.getElementById('email').value;

    if (!validateRFID(rfid)) {
        e.preventDefault();
        const errorSpan = document.getElementById('rfid-error');
        errorSpan.textContent = 'Invalid RFID. It must start with "RFID".';
        alert('Form submission blocked: Invalid RFID.');
    }

    if (!validateEmail(email)) {
        e.preventDefault();
        const errorSpan = document.getElementById('signup-email-error');
        errorSpan.textContent = 'Invalid email format.';
        alert('Form submission blocked: Invalid email.');
    }
});
