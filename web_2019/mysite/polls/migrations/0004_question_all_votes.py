# Generated by Django 3.0.1 on 2019-12-23 04:55

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('polls', '0003_auto_20191223_0651'),
    ]

    operations = [
        migrations.AddField(
            model_name='question',
            name='all_votes',
            field=models.IntegerField(default=0),
        ),
    ]