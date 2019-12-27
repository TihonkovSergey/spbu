from django.contrib import admin

from .models import Choice, Question

class ChoiceInline(admin.TabularInline):
    model = Choice
    extra = 3

class QuestionAdmin(admin.ModelAdmin):
    fieldsets = [
        (None,               {'fields': ['question_text']}),
        ('Информация о дате', {'fields': ['pub_date'], 'classes': ['collapse']}),
        ('Хотите получать уведомление на почту?',               {'fields':['sub_to_not']}),
    ]
    inlines = [ChoiceInline]
    list_display = ('question_text', 'pub_date', 'sub_to_not', 'was_published_recently')
    list_filter = ['pub_date']
    search_fields = ['question_text']

    
admin.site.register(Question, QuestionAdmin)
