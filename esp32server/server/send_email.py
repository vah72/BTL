def send_email(guess,date):
    import smtplib, ssl
    from email.message import EmailMessage

    email_sender = "hoangvananh7201@gmail.com"
    email_password = "xtpjhlfcuzlztrzy"
    email_receiver = "hoangvananh987@gmail.com"

    subject = 'Mô hình IOT dự báo thời tiết'
    body = f"""
    Weather forecast for {date} is going to  {guess} 
    """

    em = EmailMessage()
    em['From'] = email_sender
    em['To'] = email_receiver
    em['Subject'] = subject
    em.set_content(body)

    context = ssl.create_default_context()

    with smtplib.SMTP_SSL('smtp.gmail.com', 465, context=context) as smtp:
        smtp.login(email_sender, email_password)
        smtp.sendmail(email_sender, email_receiver, em.as_string())