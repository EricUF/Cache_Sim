import { Component, OnInit } from '@angular/core';
import { ContentService } from '../shared/services/content.service';
import { ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-page',
  templateUrl: './page.component.html',
  styleUrls: ['./page.component.css']
})
export class PageComponent implements OnInit {
  page = {
    title: 'A picture is worth a thousand words.',
    subtitle: 'Here you can upload images in which you can hide messages in plain sight that can decrypted using passwords.',
    content: '',
    image: 'assets/logo.jpg'
  };

  constructor(private route: ActivatedRoute,
    private contentService: ContentService) { }

ngOnInit() {
const pageData = this.route.snapshot.data['page'];
// this.page = this.contentService.pages[pageData];
}
}